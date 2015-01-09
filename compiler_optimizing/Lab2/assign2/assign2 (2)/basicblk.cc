#include "basicblk.h"
#include <iostream> 	//std::cout
#include <algorithm>	//std::sort
#include <cstddef>


using std::cout;
using std::endl;


typedef std::vector<int> int_type;


basicblk::basicblk(cfg * graph_, simple_instr * leader_instr_, simple_instr * last_instr_,
 int bb_number_, int leader_instr_number_, int last_instr_number_, bool isEntry_, bool isExit_) {

	graph = graph_;
	leader_instr = leader_instr_;
	last_instr = last_instr_;
	bb_number = bb_number_;
	leader_instr_number = leader_instr_number_;
	last_instr_number = last_instr_number_;
	isEntry = isEntry_;
	isExit = isExit_;
	idom = -1;
	inStack = FALSE;
	loophead = -1;
}



void basicblk::establish_succ_pred() {

	// Overview of what needs to be done:
	// 1) if I am the entry block then my only successor is first BB
	// 2) else if I am the exit block then I have no successor
	// 3) else if I'll go and check my last instruction which is 
	// either jump and branch instruction, return instruction or others.
	// if it is jump and branch then find the bb_number associated to the
	// traget lables and add them to my successors' list
	// if it is exit instruction then add exit block to my successors' list
	// if it is others my only successor is BB with bb_number + 1
	// meanwhile whenever I add a bb_number to my successor I'll go to basicblk
	// corresponding to that bb_number and add my bb_number to it's predecessors' list.

	int succ_bb_number;
	basicblk * bb_target;
	int target_loophead;

	if (isEntry)
	{
		succ.push_back(1);
		graph->add_pred_to_succ(bb_number, 1);
		return;
	}

	
	if (isExit)
		return; // Nothing to do


	switch(last_instr->opcode)
	{

		case JMP_OP:
		{
			// The flow control is unconditionally transferred to the code at target label
			succ_bb_number = graph->get_bb_number(last_instr->u.bj.target);

			succ.push_back(succ_bb_number);
			graph->add_pred_to_succ(bb_number, succ_bb_number);
			break;

		}


		case BTRUE_OP:
		case BFALSE_OP:
		{
			// The flow control is transferred to the code at the target label. 
			// Otherwise it continues with the next instruction in sequential order

			succ_bb_number = graph->get_bb_number(last_instr->u.bj.target);	
	

			succ.push_back(succ_bb_number);
			graph->add_pred_to_succ(bb_number, succ_bb_number);

			succ_bb_number = bb_number + 1; // Need to do error checking not going out of bound
			if (succ_bb_number < graph->get_n())
			{
			
				succ.push_back(succ_bb_number); 
				graph->add_pred_to_succ(bb_number, succ_bb_number);
			}
			
			break;
		} 


		case MBR_OP:
		{
			succ_bb_number = graph->get_bb_number(last_instr->u.mbr.deflab);
		

			succ.push_back(succ_bb_number);
			graph->add_pred_to_succ(bb_number, succ_bb_number);

			int i = 0;
			for (i = 0; i < last_instr->u.mbr.ntargets; i++)
			{
				succ_bb_number = graph->get_bb_number(last_instr->u.mbr.targets[i]);
				

				succ.push_back(succ_bb_number);
				graph->add_pred_to_succ(bb_number, succ_bb_number);
			}
			break;
		}


		case RET_OP:
		{
			// add exit to it's successor's list
			succ_bb_number = (graph->get_n()) - 1;
			succ.push_back(succ_bb_number);
			graph->add_pred_to_succ(bb_number, succ_bb_number);
			break;
		}


		default:
		{
			succ_bb_number = bb_number + 1; // Need to do error checking not going out of bound
			if (succ_bb_number < graph->get_n())
			{
			
				succ.push_back(succ_bb_number); 
				graph->add_pred_to_succ(bb_number, succ_bb_number);
			}
			break;
		}
			
	}

}



void basicblk::add_pred(int pred_num)
{
	// add pred_num to my predeccessors list
	pred.push_back(pred_num);
}



void basicblk::print_basicblk()
{
	// prints basic block information in required fromat
	cout<<"block "<< bb_number << endl; 
	print_instr();
	print_succ();
	print_pred();
}



void basicblk::print_idom()
{
	// prints my immidiate dominator 
	cout<<"block "<< bb_number << endl;
	
	if (idom >= 0)
		cout<<"\tidom "<<idom<<endl;
	else
		cout<<"\tidom "<< endl;

}



void basicblk::print_instr()
{
	// prints the instructions and their numbering
	if(isEntry || isExit)
	{
		cout << "\t"<<"instrs 0"<<endl;
		return;
	}

	int instrs_num = (last_instr_number - leader_instr_number) +1;

	cout << "\t"<<"instrs "<<instrs_num;
	int i;
	for (i = leader_instr_number; i <= last_instr_number; i++)
		cout<<" "<< i ;

	cout<<endl;	
}



void basicblk::print_succ()
{
	// prints my successors in required format
	int succ_num = succ.size();
	std::sort(succ.begin(), succ.end());
	

	cout << "\t" << "successors " << succ_num;
	int i;
	for (i = 0; i < succ_num; i++)
	{
		cout<<" "<<succ[i];
	}
	cout<<endl;
}



void basicblk::print_pred()
{
	// prints my predecessors in required format
	int pred_num = pred.size();
	std::sort(pred.begin(), pred.end());

	cout << "\t" << "predecessors " << pred_num;
	int i;
	for(i = 0; i < pred_num; i++)
	{
		cout<<" "<<pred[i];
	}

	cout<<endl;
}



void basicblk::initialize_dom()
{
	// Initialize my set of dominators
	// if I am the entry block my dom set contains only me
	// otherwise my dom set contains every other node including me
	if (isEntry)
	{
		dom.push_back(0);
		return;
	}

	int i;
	for (i = 0; i < graph->get_n(); i++)
		dom.push_back(i);

	return;
}



int_type basicblk::get_pred()
{
	// return my predeccessor list
	return pred;
}

int_type basicblk::get_succ()
{
	//return my successor list
	return succ;
}



int_type basicblk::get_dom()
{
	// return my dominator list
	return dom;
}



void basicblk::set_dom(int_type dom_)
{
	// set my dominator
	dom = dom_;
}



void basicblk::find_idom()
{
	// Iterative algorithm to find dominators for all vertices.
	int i, j;
	bool found = FALSE;
	int dom_size = dom.size();

	for (i = 0; i < dom_size; i++)
	{
		
		if (dom[i] == bb_number)
			continue;

		found = TRUE;

		for (j = 0; j < dom_size; j++)
		{
			if (dom[j]==bb_number)
				continue;

			if (!graph->is_v1_dom_v2(dom[j], dom[i])) 	// if dom[j] does not dominates dom[i] then dom[i] can not be immidiate dominator
			{
				found = FALSE;
				break;
			}
		}

		if (!found)
			continue;
		break;
	}
	
	if (found)
		idom = dom[i];
}



bool basicblk::is_dom_me(int v)
{
	// return true if v dominates me
	int i;
	int dom_size = dom.size();

	for (i = 0; i < dom_size; i++)
		if (dom[i] == v)
			return TRUE;

	return FALSE;
}



bool basicblk::isReachable()
{
	// returns true if the node is reachable
	if (isEntry)
		return TRUE;

	if (dom.size() == 1)
		return FALSE;
	return TRUE;
}



void basicblk::set_loophead(int loopnumber)
{	
	// set loopnumber in which I am the head
	loophead = loopnumber;
}



int basicblk::get_loophead()
{
	// return loophead number
	return loophead;
}



void basicblk::set_bb_number (int bb_number_)
{
	// set bb_number
	bb_number = bb_number_;
}



int basicblk::get_bb_number ()
{
	// return my basic block number
	return bb_number;
}



void basicblk::add_to_looptails(int loopnumber)
{
	// add loop number where I am a tail in that loop
	looptails.insert(loopnumber);
}



simple_instr * basicblk::get_leader_instr()
{
	// return leader instruction
	return leader_instr;
}



simple_instr * basicblk::get_last_instr()
{
	// return last instruction
	return last_instr;
}

