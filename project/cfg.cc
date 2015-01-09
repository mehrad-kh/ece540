#include "cfg.h"
#include "basicblk.h"
#include "dfa.h"
#include <cstddef>
#include <iostream>
#include <assert.h>
#include <algorithm>	//std::sort
extern "C" { 
#include <simple.h>
}


using std::cout;
using std::endl;


typedef std::vector<int> int_type;
typedef std::vector < boost::dynamic_bitset <> > bitset_vector;


simple_instr * cfg::build_cfg(simple_instr *inlist_)
{
	vertexlist.clear();
	lab_map_bbnum.clear();
	looplist.clear();
	variablelist.clear();
	variable_map_varnum.clear();

	inlist = inlist_;
	build_cfg_helper();
	return inlist;
}



void cfg::build_cfg_helper()
{
	// A Whole lot of things are going on here
	// 1) This function goes thorough the instruction lists
	//    and identifies all leader instructions.
	// 2) Finds all basic blocks
	// 3) Then calls set edges to find all edges in the graph. 
	// 4) Finally invokes find_doms() to find all dominators. 


	if (!inlist)
		return;

	/* Creating entry block */

	basicblk *new_bb = new basicblk(this, NULL, NULL, 0, -1, -1, TRUE, FALSE);
	vertexlist.push_back(new_bb);

	/* traverse the list and find all basic blocks */

	int bb_number = 1; 	// hold basic block number that is going to add to vertexlist
	int leader_instr_number = 0;
	int last_instr_number = 0;

	simple_instr *curr = inlist;
	simple_instr *leader_inst = inlist;
	simple_instr *last_inst = NULL;

	int counter = 0;	// incremented in each iteration of while loop 
	int var_num = 0;	// variable number starts from 1
	int expr_num = 0;  	// 

	// check if the first instruction's opcode is LABEL_OP
	// if it is then add the mapping to lab_map_bbnum 
	 if (inlist->opcode == LABEL_OP)
	 {
	 	lab_map_bbnum[inlist->u.label.lab] = 1;
	 	curr = inlist->next;
	 	counter = 1;
	 }
	 	
	 bool begin = TRUE;

	while (curr)
	{

		//number_instr_var(curr, var_num); // add variable of this instruction if any into variablelist 

		if (curr == leader_inst && ! begin && curr->opcode == LABEL_OP)
		{
			counter++;
			curr = curr->next;
			continue;
		}

		begin = FALSE;

		switch(curr->opcode)
		{

			case JMP_OP:
			case BTRUE_OP:
			case BFALSE_OP:
			case MBR_OP: 
			case RET_OP:
			{
				last_inst = curr;
				last_instr_number = counter;
				new_bb = new basicblk(this, leader_inst, last_inst, bb_number, 
					leader_instr_number,last_instr_number, FALSE, FALSE);
				vertexlist.push_back(new_bb);
				leader_inst = curr->next;
				leader_instr_number = counter + 1;
				last_inst = NULL;
				bb_number++;
				// if new leader instrction is label thenstore 
				// mapping of label to its basic block number
				// needed for finding successors and predecessors
				if (leader_inst && leader_inst->opcode == LABEL_OP)
					lab_map_bbnum[leader_inst->u.label.lab] = bb_number;
				break;
			}

			case LABEL_OP:
			{
				last_inst = curr->prev;
				last_instr_number = counter-1;
				new_bb = new basicblk(this, leader_inst, last_inst, bb_number,
				 	leader_instr_number,last_instr_number ,FALSE, FALSE);
				vertexlist.push_back(new_bb);
				leader_inst = curr;
				leader_instr_number = counter;
				last_inst = NULL;
				bb_number++;
				// store mapping of label to its basic block number
				// needed for finding successors and predecessors
				lab_map_bbnum[curr->u.label.lab] = bb_number;  
				break;
			}

			default:
				break;	
		}

		counter++;
		curr = curr->next;
	}

	// Do extra check later that all blocks have been completely set

	/* Creating exit block */

	new_bb = new basicblk(this, NULL, NULL, bb_number, -1, -1, FALSE, TRUE);
	vertexlist.push_back(new_bb);

	n = vertexlist.size();

	// Once all the basicblks are identified we can go and find edegs between them
	set_edges();
	// at this time all vertices and edges are fully identified.
	// so we can go and find all dominators for every vertex.
	find_doms();

	//find_local_lvs();

	//lv_solver();

	return;
}


void cfg::set_my_dfa(dfa * mydfa_)
{
	mydfa = mydfa_;
}


void cfg::set_my_use_def(use_def * my_use_def_)
{
	my_use_def = my_use_def_;
}


void cfg::set_edges()
{
	// set all edges between basic blocks
	int i;
	for (i = 0; i < n; i++)
	{
		vertexlist[i]->establish_succ_pred();
	}

}



int cfg::get_bb_number(simple_sym * lab)
{
	// return bb_number associated to label
	return lab_map_bbnum[lab];
}



int cfg::get_n()
{
	// return number of nodes
	return n;
}



void cfg::add_pred_to_succ(int pred_num, int succ_num)
{
	if (pred_num > n || succ_num > n)
		return;

	vertexlist[succ_num]->add_pred(pred_num);
	return;

}



void cfg::print_cfg(char * proc_name) 
{
	int i = 0;
	// print the whole control flow graph
	//process_loops();
	
	/*std::cout<< "loopcfg " << proc_name << " " << n << "\n";

	
	for (i = 0; i < n; i++)
	{
		vertexlist[i]->print_basicblk();
	} */

	// For lab1 
	/*std::cout<< "idominators " << proc_name << " " << n << "\n";
	for (i = 0; i < n; i++)
	{
		vertexlist[i]->print_idom(); 
	} */

	

	//print_variable_list(proc_name);
	//print_lv_set(proc_name);
	
}

void cfg::print_loops(char * proc_name)
{
	int i;
	// print natloops
	std::cout<< "natural_loops " << proc_name << " " << natloop_num << "\n";

	for (i = 0; i < natloop_num; i++)
	{
		looplist[i]->print_natloop();
	} 
}



void cfg::find_doms()
{
	//Overview of what needs to be done
	//1) initialize all dominators
	//2) For all vertices except entry and exit node V
	//3)	olddom = copy of dom(v)
	//4)	get predecessors of v
	//5)	if pred(v) not empty
	//6)		newdom = pred(0).dom
	//7)	for all pred(v) (0<i<pred_num)
	//8)		newdom = intersection(newdom, pred(i).dom)
	//9)	add v to newdom
	//10)	set v.dom to newdom

	int i, j;

	// Initialize all dominators
	for(i = 0; i < n; i++)
		vertexlist[i]->initialize_dom();

	bool changed = TRUE;;
	int_type olddom;
	int_type newdom;
	int_type pred;

	while(changed)
	{
		changed = FALSE;
		for (i = 1; i < n; i++)
		{
			olddom = vertexlist[i]->get_dom();
			pred = vertexlist[i]->get_pred();
			//cout << "here 3\n";
			int k = 0;
			if (!pred.empty())  // if not empty
			{
				while (k < pred.size() && !vertexlist[pred[k]]->isReachable())
					k++;

				if (k < pred.size())
					newdom = vertexlist[pred[0]]->get_dom();
				else
					newdom.clear();
			}
			else
				newdom.clear();
			
			for(j = k + 1 ; j < pred.size(); j++)
				if (vertexlist[pred[j]]->isReachable())
					newdom = set_intersection(newdom, vertexlist[pred[j]]->get_dom());

			newdom.push_back(i); 	// add v to its dominator set
			vertexlist[i]->set_dom(newdom);

			if (olddom != newdom)
				changed = TRUE;
		}
	}

	// At this point all dominators are fully identified for each basic block
	// Now for each basic block we call find_idom to determine immidiate dominator
	// of each basic block
	for (i = 0; i < n; i++)
		vertexlist[i]->find_idom();
}



int_type cfg::set_intersection(int_type set1, int_type set2)
{
	// receive two vector of int (set1 and set2) as an argument
	// both are in increasing order. The intesection of them
	// is computed as will be stored in inter_set.

	int i, j;
	i = 0;
	j = 0;
	int_type inter_set;
	while (i < set1.size() && j < set2.size())
	{
		if (set1[i] == set2[j])
		{
			inter_set.push_back(set1[i]);
			i++;
			j++;
		}

		else if (set1[i] < set2[j])
		{
			i++;
		}

		else
		{
			j++;
		}
	}

	return inter_set;
}



bool cfg::is_v1_dom_v2(int v1, int v2)
{
	//returns true if v1 dominates v2
	return (vertexlist[v2]->is_dom_me(v1));
}



basicblk * cfg::get_basicblk_addr (int i)
{
	// returns a pointer to basickblk i;
	return vertexlist[i];
}



void cfg::process_loops()
{
	// This is the function to start processing loops.
	// The overview of aaction taken here
	// 1) Invokes DFS to find all back edges and get the result in back_edge map
	// 2) Invokes insert_instruction to insert all required new instruction into the inlist
	// 3) Destroys all previous basicblks and invokes build_cfg_helper to build cfg again
	// 4) Then invokde DFS again to find backedges with correct numbering  
	// 5) After that sets up looplist
	// 6) Then go through the looplist and for each element calls find_loop_block 
	//	  to find all nodes constituting the loop

	int i;
	std::tr1::unordered_map<int, int_type> back_edge; 
	std::tr1::unordered_map<int, int_type>::iterator it;	// iterator

	DFS(back_edge);		// invoke DFS 
	natloop_num = back_edge.size();
	
 	int_type loop_heads;

	for (it = back_edge.begin(); it != back_edge.end(); ++it)
		loop_heads.push_back(it->first);
	
	std::sort(loop_heads.begin(), loop_heads.end());
	
	// Insert all new required instruction into the inlist
	insert_instruction(loop_heads, back_edge);
	
	// Now destroy all basic blocks 
	for (i = 0; i < n; i++)
	{
		delete vertexlist[i];
	}

	vertexlist.clear();
	lab_map_bbnum.clear();

	back_edge.clear();
	loop_heads.clear();

	// build cfg again
	build_cfg_helper();

	// find back edges with correct numbering
	DFS(back_edge);
	natloop_num = back_edge.size();

	// Set up looplist
	for (it = back_edge.begin(); it != back_edge.end(); ++it)
		loop_heads.push_back(it->first);
	
	std::sort(loop_heads.begin(), loop_heads.end());
	
	for (i = 0; i < natloop_num; i++)
	{
		natloop * newloop = new natloop(this, mydfa, my_use_def, loop_heads[i], back_edge[loop_heads[i]], i);
		looplist.push_back(newloop);
	}
	
	// for each natural loop find all the nodes constituting that
	for (i = 0; i < natloop_num; i++)
	{
		looplist[i]->find_loop_blocks();
	} 	

}



void cfg::DFS (std::tr1::unordered_map<int, int_type> & back_edge)
{
	// Perform DFS traversal of cfg and fills out back_edge dictionary
	std::set<int> vistedSet;  

	int i;
	for (i = 0; i < n; i++)
	{
		if (vistedSet.find(i) == vistedSet.end())
		{
			vistedSet.insert(i);
			DFS_visit(back_edge, vistedSet, i);
		}
	}
}



void cfg::DFS_visit(std::tr1::unordered_map<int, int_type> & back_edge, 
	std::set<int> & vistedSet, int v)
{
	vertexlist[v]->inStack = TRUE;		// start visiting v

	int_type successor = vertexlist[v]->get_succ();
	int out_degree = successor.size();


	int i;
	for (i = 0; i < out_degree; i++)
	{
		if (vistedSet.find(successor[i]) == vistedSet.end())   // tree edge
		{
			
			vistedSet.insert(successor[i]);
			DFS_visit(back_edge, vistedSet, successor[i]);

		}	
		else if (vertexlist[successor[i]]->inStack == TRUE)	//Back edge
		{
			if (is_v1_dom_v2(successor[i], v))
				back_edge[successor[i]].push_back(v);

		}
	}

	vertexlist[v]->inStack = FALSE;		// finish visiting v
	
	return;

}



void cfg::insert_instruction(int_type & loop_heads, std::tr1::unordered_map<int, int_type> & back_edge )
{
	// This function insters new instruction into the inlist.
	// loops over all the loopheads. For each loop head insert a 
	// new label instruction as a preheader instruction right before 
	// loophead's leader instruction.
	// Then for each loophead gets its predeccessors.
	// There are 2 types of predeccessors.
	// Type 1) predeccessor is a tail for that loophead
	//		In this case we check if the tail was transfering the control to loophead
	//		through a fall through then we need to add a new jump instruction to end of 
	//		the tail block in order to jump directly too loophead. Becuase otherwise,
	//		it would go to preheader block.
	// Type 2) predeccessor is not tail for that loophead
	//		 In this case we check if last instruction of the predeccessor is a jump 
	//		 and branch instruction and targeting loophead then we change it to target
	//		 preheader.	

	int i;
	int loop_heads_num = loop_heads.size();

	for (i = 0; i < loop_heads_num; i++ )
	{
		simple_instr * head_leader_instr = vertexlist[loop_heads[i]]->get_leader_instr();
		simple_sym * head_label = head_leader_instr->u.label.lab;

		if (head_leader_instr->opcode == LABEL_OP)
		{
			// Insert a new lable instruction just before loophead's leader instruction 
			simple_instr * preheader_instr = new_instr(LABEL_OP, simple_type_void);
			simple_sym * preheader_label = new_label();
			preheader_instr->u.label.lab = preheader_label;

			// inserting new instruction into the list
			if (head_leader_instr->prev)
			{
				// prev instruction is not null
				preheader_instr->next = head_leader_instr;
				preheader_instr->prev = head_leader_instr->prev;
				head_leader_instr->prev->next = preheader_instr;
				head_leader_instr->prev = preheader_instr;
			}

			else 
			{
				// prev instruction is null
				// head_leader_inst is the first instruction of inlist
				preheader_instr->next = head_leader_instr;
				preheader_instr->prev = NULL;
				head_leader_instr->prev = preheader_instr;
				inlist = preheader_instr;

			}

			int_type pred = vertexlist[loop_heads[i]]->get_pred();
			int pred_size = pred.size();
			int j;

			for (j = 0; j < pred_size; j++)
			{
				if (std::find(back_edge[loop_heads[i]].begin(), back_edge[loop_heads[i]].end(), pred[j]) != back_edge[loop_heads[i]].end())
				{
					// if this predeccessor is in tails list
					simple_instr * tail_last_instr = vertexlist[pred[j]]->get_last_instr();
					if (tail_last_instr == NULL)
					{
						// This case should never happen but I put it here as a precaution
						// since tail of backege cannot be entry block
						continue;
					}

					if (tail_last_instr->opcode != JMP_OP && tail_last_instr->opcode != MBR_OP)
					{
						if (tail_last_instr->opcode == BTRUE_OP || tail_last_instr->opcode == BFALSE_OP)
						{
							if (tail_last_instr->u.bj.target != head_label)
							{

								simple_instr *jump_instr = new_instr(JMP_OP, simple_type_address);
								jump_instr->u.bj.target = head_label;
								simple_reg * src = new_register(simple_type_address, PSEUDO_REG);
								jump_instr->u.bj.src = src;

								if (tail_last_instr->next)
								{
									// next instruction is not null 
									jump_instr->prev = tail_last_instr;
									jump_instr->next = tail_last_instr->next;
									tail_last_instr->next->prev = jump_instr;
									tail_last_instr->next = jump_instr;
								}

								else
								{
									// next instruction is null
									jump_instr->prev = tail_last_instr;
									jump_instr->next = NULL;
									tail_last_instr->next = jump_instr;
								}
							}
						}

						else
						{
							
							simple_instr *jump_instr = new_instr(JMP_OP, simple_type_address);
							jump_instr->u.bj.target = head_label;
							simple_reg * src = new_register(simple_type_address, PSEUDO_REG);
							jump_instr->u.bj.src = src;

							if (tail_last_instr->next)
							{
								// next instruction is not null 
								jump_instr->prev = tail_last_instr;
								jump_instr->next = tail_last_instr->next;
								tail_last_instr->next->prev = jump_instr;
								tail_last_instr->next = jump_instr;
							}

							else
							{
								// next instruction is null
								jump_instr->prev = tail_last_instr;
								jump_instr->next = NULL;
								tail_last_instr->next = jump_instr;
							}
						}
					}
				}

				else
				{
					// if predeccessor is NOT in tail
					simple_instr * pred_last_instr = vertexlist[pred[j]]->get_last_instr();

					if (pred_last_instr == NULL)
					{
						// Nothing to do
						continue;
					}

					if (pred_last_instr->opcode == JMP_OP)
					{
						// change the target to preheader label
						pred_last_instr->u.bj.target = preheader_label;
					}

					else if (pred_last_instr->opcode == BTRUE_OP || pred_last_instr->opcode == BFALSE_OP)
					{
						if (pred_last_instr->u.bj.target == head_label)
						{
							// change the target label to preheader label
							pred_last_instr->u.bj.target = preheader_label;
						}
					}

					else if (pred_last_instr->opcode == MBR_OP)
					{
						if (pred_last_instr->u.mbr.deflab == head_label)
						{
							// change the target label to preheader label
							pred_last_instr->u.mbr.deflab = preheader_label;
						}

						int k;
						for (k = 0; k < pred_last_instr->u.mbr.ntargets; k++)
						{
							if (pred_last_instr->u.mbr.targets[k] == head_label)
							{
								// change the target label to preheader label
								pred_last_instr->u.mbr.targets[k] = preheader_label;
							}
						}
					}
				}
			}

		}

		else 
		{
			//cout << "This case should not happen" << endl;
			//cout << "I believe all loop's head's firt instruction has LABEL_OP format !!!!" << endl;;
		}
	}

}


simple_instr * cfg::get_inlist()
{
	// return inlist
	return inlist;
}


void cfg::LICM_optimization ()
{
	int i;

	//sort(looplist.begin(), looplist.end(), way_to_sort);

	for (i = 0; i < natloop_num; i++)
	{
		looplist[i]->LICM();
	} 	

	cout << "************************************the order is going to perform LICM is:\n";
	//std::vector<natloop *>::iterator it;
	//for(it = looplist.begin(); it != looplist.end(); it++)
	for (i = 0; i < natloop_num; i++)
	{
		cout << looplist[i]->loop_number << "   ";
	}

}

bool cfg::way_to_sort(natloop * i, natloop * j)
{
	int i_head = i->head->get_bb_number();
	int j_head = j->head->get_bb_number();

	int_type i_dom = i->head->get_dom();

	int k = 0;
	int i_dom_size = i_dom.size();
	for (k = 0; k < i_dom_size; k++)
	{
		if (i_dom[k] == j_head)
			return TRUE;
	}

	return FALSE;

}
