#include "cfg.h"
#include "basicblk.h"
#include <cstddef>
#include <iostream>


using std::cout;
using std::endl;


typedef std::vector<int> int_type;



void cfg::build_cfg (simple_instr *inlist)
{
	// A Whole lot of things are going on here
	// 1) This function goes thorough the instruction lists
	//    and identifies all leader instructions.
	// 2) Finds all basic blocks
	// 3) Then calls set edges to find all edges in the graph. 
	// 4) Finally invokes find_doms() to find all dominators. 


	//std::cout <<"start creating a graph\n";
	if (!inlist)
		return;

	/* Creating entry block */

	basicblk *new_bb = new basicblk(this, NULL, NULL, 0, 0, 0, TRUE, FALSE);
	vertexlist.push_back(new_bb);

	/* traverse the list and find all basic blocks */

	int bb_number = 1; 	// hold basic block number that is going to add to vertexlist
	int leader_instr_number = 0;
	int last_instr_number = 0;

	simple_instr *curr = inlist;
	simple_instr *leader_inst = inlist;
	simple_instr *last_inst = NULL;

	int counter = 0;	// incremented in each iteration of while loop 

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
		if (curr == leader_inst && ! begin){
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

	new_bb = new basicblk(this, NULL, NULL, bb_number, 0, 0, FALSE, TRUE);
	vertexlist.push_back(new_bb);

	n = vertexlist.size();

	// Once all the basicblks are identified we can go and find edegs between them
	set_edges();
	// at this time all vertices and edges are fully identified.
	// so we can go and find all dominators for every vertex.
	find_doms();
	return;
}



void cfg::set_edges()
{
	// set all edges between basic blocks
	int i = 0;
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
	// print the whole control flow graph
	std::cout<< "cfg " << proc_name << " " << n << "\n";

	int i = 0;
	for (i = 0; i < n; i++)
	{
		vertexlist[i]->print_basicblk();
	}

	std::cout<< "idominators " << proc_name << " " << n << "\n";
	for (i = 0; i < n; i++)
	{
		vertexlist[i]->print_idom();
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
			//cout << "here 4\n";
			
			for(j = k + 1 ; j < pred.size(); j++)
				if (vertexlist[pred[j]]->isReachable())
					newdom = set_intersection(newdom, vertexlist[pred[j]]->get_dom());
			//cout << "here 5\n";

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