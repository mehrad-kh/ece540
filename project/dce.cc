#include "cfg.h"
#include "basicblk.h"
#include "dfa.h"
#include <stdio.h>
#include <cstddef>
#include <iostream>
#include <assert.h>
#include <algorithm>	//std::sort
extern "C" { 
#include <simple.h>
}
extern "C" { 
#include "print.h"
}


using std::cout;
using std::endl;






simple_instr * cfg::remove_dead_code()
{
	// clear out all unreachable blocks;
	// make all instruction in those block to NOP_OP; 
    mark_nop_unreachable_block();

    perform_dce();
    //cout << "done performing dce\n";
    //fprint_instr(stdout, inlist, 0);
    remove_jmps();
    remove_nops();

    return inlist;

}


void cfg::mark_nop_unreachable_block()
{
	// for each basic block if it is unreachable
	// then mark all of its intruction to be NOP_OP
	int i;
	for (i = 0; i < n; i++)
	{
		if(!vertexlist[i]->isReachable())  // if not reachable
		{
			simple_instr * leader_instr = vertexlist[i]->get_leader_instr();
			simple_instr * last_instr = vertexlist[i]->get_last_instr();
			if(leader_instr == NULL)
				continue;

			simple_instr * s = leader_instr;
			while(s != last_instr->next)
			{
				s->opcode = NOP_OP;
				s = s->next;
			}
		}
	}
}




void cfg::worklist_initial_essential_instr(basicblk *bb, worklist &wl) 
{
	// go find all instructions that allow variables to output form
	// the function or change memory storage
	// consider those instructions to be essential
	simple_instr * leader_instr = bb->get_leader_instr();
	simple_instr * last_instr = bb->get_last_instr();

    if(leader_instr == NULL) 
        return;
    
    simple_instr * s = leader_instr;

    while(s != last_instr->next)
    {
    	switch(s->opcode)
    	{
    		case RET_OP:
    		case STR_OP:
    		case MCPY_OP:
    		case CALL_OP:
    		{
    			wl.push_back(worklist_item(bb, s));
    			break;
    		}
    		
    		default: break;
    	}

    	s = s->next;
    }
}


void cfg::mark_label_initial_essential_instr(std::set<simple_instr *> & essential_instr)
{
	/// go find each label instruction and mark it as essential instruction
	// don't need to add them to worklist since none of them has any uses.
	simple_instr * s = inlist;
	while(s)
	{
		if(s->opcode == LABEL_OP)
		{
			essential_instr.insert(s);
		}
		s = s->next;
	}
}



void cfg::perform_dce()
{
	worklist myworklist;
	std::set<simple_instr *> essential_instr;
    std::set<basicblk *> control_dependent;

    //initialize my worklist
    std::vector<basicblk *>::iterator it;
    for (it = vertexlist.begin(); it != vertexlist.end(); it++)
    {
    	worklist_initial_essential_instr((*it), myworklist);
    }

    //initialize essential instruction with labels
    // don't need to initialize my worklist with them
    // since none of them has any use
    mark_label_initial_essential_instr(essential_instr);

    bool change = TRUE;
    while (change)
    {   
        change = FALSE;
    	control_dependent.clear(); // clear out control dependent blocks

    	while(!myworklist.empty())
    	{
    		worklist_item new_item = myworklist.back();
    		myworklist.pop_back();

            // check if new_item already exit in the essential_instr
            if (essential_instr.find(new_item.instr) != essential_instr.end())
            {
                continue;
            }

            else
            {
                // go and get the list of predecessor of basic block corresponding to
                // new item. and add it to control_dependent set.
                std::vector<int> predecessors = new_item.bb->get_pred();
                std::vector<int>::iterator pit;
                for(pit = predecessors.begin(); pit != predecessors.end(); pit++)
                {
                    control_dependent.insert(vertexlist[(*pit)]);
                }
            }
            //cout << "instruction from worklist\n";
            //fprint_instr(stdout, new_item.instr, 0);
            essential_instr.insert(new_item.instr);

            // go and get list of instruction that reaches new added instruction operands
            // add them to the worklist since they are essentials
            instr_usage_reaching_def(new_item.instr, new_item.bb, myworklist);
    	}
        //now find branches that has essential instruction control dependent on it.
        // worklist is empty here if something gets added then make change = TRUE
        std::set<basicblk *> all_cds; // all control dependencies
        std::set<basicblk *> next_cds; // next control dependencies
        bool change_cd = TRUE;
        while(change_cd)
        {
            change_cd = FALSE;
            std::set<basicblk *>::iterator bbit;

            for (bbit = control_dependent.begin(); bbit != control_dependent.end(); bbit++)
            {
                basicblk *bb = (*bbit);


                // entry/exit block, or already found this block as being
                // control dependeny
                if(0 == bb->get_leader_instr() || 0 != all_cds.count(bb)) 
                {
                    continue;
                }

                all_cds.insert(bb);

                switch(bb->get_last_instr()->opcode) 
                {
                    // direct control dependencies
                    case JMP_OP: 
                    case BTRUE_OP: 
                    case BFALSE_OP: 
                    case MBR_OP:
                    
                    {
                        change = true;
                        myworklist.push_back(worklist_item(bb, bb->get_last_instr()));
                        break;
                    }

                    // handle cascading control dependencies through fall-throughs
                    default: 
                    {
                        change_cd = TRUE;
                        
                        std::vector<int> preds = bb->get_pred();
                        std::vector<int>::iterator predsit;
                        for(predsit = preds.begin(); predsit != preds.end(); predsit++)
                        {
                            next_cds.insert(vertexlist[(*predsit)]);
                        }

                        break;
                    }
                }
            }
            //
            control_dependent.swap(next_cds);
            next_cds.clear();
        }
    
    }
    // change every non-essential instruction to NOP op. 
    nop_out_non_essentials_instr(essential_instr);
}


void cfg::nop_out_non_essentials_instr(std::set<simple_instr *> & essential_instr)
{
    // go through the inlist. For each instruction check if it has not appeared in the
    // essential_instr set then change its opcode to NOP_OP.
    simple_instr * curr = inlist;
    while(curr)
    {
        if(essential_instr.count(curr) == 0)
        {
            curr->opcode = NOP_OP;
        }
        curr = curr->next;
    }
}

void cfg::instr_usage_reaching_def(simple_instr * s, basicblk * bb, std::vector<worklist_item> &wl)
{
    cout <<"in instr_usage_reaching_def\n";
    fprint_instr(stdout, s, 10);
    switch(simple_op_format(s->opcode))
    {
        case BASE_FORM:
        {
            opnd_reaching_def(s, bb, s->u.base.src1, wl); 
            opnd_reaching_def(s, bb, s->u.base.src2, wl); 
            break;
        }

        case BJ_FORM:
        {
            opnd_reaching_def(s, bb, s->u.bj.src, wl); 
            break;
        }

        case LDC_FORM:
        {

            break;
        }

        case CALL_FORM:
        {
            //cout << "in call form?\n";
            opnd_reaching_def(s, bb, s->u.call.proc, wl);

            int nargs = s->u.call.nargs;
            for (int i = 0; i < nargs; i++)
            {
                opnd_reaching_def(s, bb, s->u.call.args[i], wl);
            }
            break;
        }

        case MBR_FORM:
        {
            opnd_reaching_def(s, bb, s->u.mbr.src, wl);
            break; 
        }

        case LABEL_FORM:
        {
            break;
        }

        default:
        {
           break;// cout << "chsould nt hapeen\n";
        }
    }
}


void cfg::opnd_reaching_def(simple_instr * s, basicblk * bb, simple_reg *opnd,
 std::vector<worklist_item> &wl)
 {
   cout <<"in opnd_reaching_def\n";
   fprint_instr(stdout, s, 200);
    if(opnd == NO_REGISTER)
    {
        return;
    }

    if(opnd->kind == TEMP_REG)
    {
        simple_instr * in = s->prev;
        while(1)
        {   
            switch(simple_op_format(in->opcode))
            {
                case BASE_FORM:
                {
                    if(in->u.base.dst == opnd)
                    {
                        wl.push_back(worklist_item(bb, in));
                        return;
                    }
                    break;
                }

               case  BJ_FORM:
                {
                    break;
                }

                case LDC_FORM:
                {
                    
                    if(in->u.ldc.dst == opnd)
                    {
                        wl.push_back(worklist_item(bb, in));  
                        return;
                    }
                    break;
                }

                case CALL_FORM:
                {
                    if(in->u.call.dst == opnd)
                    {
                        wl.push_back(worklist_item(bb, in));  
                        return;
                    }
                    break;
                }

                case MBR_FORM:
                {
                    break;
                }

                case LABEL_FORM:
                {
                    break;
                }
            }

            in = in->prev;
        }
    }


    // otherwise opnd is of PSEUDO_REG which its reaching definition is 
    // captured in use_def_chain
    std::string key;
    std::vector<int> deflist;   

    char buffer[20];

    sprintf(buffer,"%x#%d", s, opnd->num);
    key.assign(buffer);

    deflist = my_use_def->use_def_chain[key];

    // deflist contains all definition number reaching this opnd.
    std::vector<int>::iterator defit;
    for (defit = deflist.begin(); defit != deflist.end(); defit++)
    {
      wl.push_back(worklist_item(vertexlist[(mydfa->defnum_map_definfo[(*defit)]).bb_number], (mydfa->defnum_map_definfo[(*defit)]).instr));  
    }
    return;
 }



 /// remove all NOPs
void cfg::remove_nops()  
{
    simple_instr * s = inlist;
    simple_instr * s_runner = inlist;
    while(s)
    {
        if(s->opcode == NOP_OP)
        {
            s_runner = s->next;
            while(s_runner && s_runner->opcode == NOP_OP)
                s_runner = s_runner->next;


            // after while loop to cases happen
            // 1) s_runner reaches end of the list
            // 2) s_runner point to non NOP instruction

            if (s_runner == NULL)
            {
                if(s->prev == NULL) // this case mean all instructions are NOP
                    inlist = NULL;
                else
                    s->prev->next = s_runner;
            }

            else
            {
                if(s->prev == NULL)
                {
                    inlist = s_runner;
                    s_runner->prev = NULL;
                }

                else
                {
                    s->prev->next = s_runner;
                    s_runner->prev = s->prev;
                }
            }

            s = s_runner;
        }

        else
        {
            s = s->next;
        }
    }
}


void cfg::remove_jmps()
{
    simple_instr * s = inlist;
    simple_instr * next;

    while(s)
    {
        next = s->next;
        if (next && s->opcode == JMP_OP && next->opcode == LABEL_OP && s->u.bj.target == next->u.label.lab)
        {
            s->opcode = NOP_OP;
        }
        s = s->next;
    }
}
