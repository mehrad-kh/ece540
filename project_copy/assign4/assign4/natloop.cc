#include "natloop.h"
#include <iostream>
#include <stack>
#include <algorithm>	//std::sort


using std::cout;
using std::endl;


typedef std::vector<int> int_type;


natloop::natloop(cfg * graph_, int head_, std::vector<int> & tails_, int loop_number_)
{
	// Constructor: Set all data members 
	int i;
	graph = graph_;
	loop_number = loop_number_;
	
	head = graph->get_basicblk_addr(head_);
	
	head->set_loophead(loop_number);

	int tails_num = tails_.size();


	for (i = 0; i < tails_num; i++)
	{
		basicblk * newtail = graph->get_basicblk_addr(tails_[i]);
		tails.push_back(newtail);
		newtail->add_to_looptails(loop_number);
	}
		
}



void natloop::find_loop_blocks()
{
	// // find all nodes constituting my loop
	std::stack<int> S;
	std::set<int> loop_blocks_set;
	loop_blocks_set.insert(head->get_bb_number());
	loop_blocks.push_back(head->get_bb_number());

	int tails_size = tails.size();

	int i;
	for (i = 0; i < tails_size; i++)
	{
		insert_on_stack(loop_blocks_set, S, tails[i]->get_bb_number());
	} 

	int_type pred;

	while (!S.empty())
	{
		int v = S.top(); 
		S.pop();
		basicblk * node = graph->get_basicblk_addr(v);
		pred = node->get_pred();

		int pred_size = pred.size();

		int j;
		for (j = 0; j < pred_size; j++)
		{
			insert_on_stack(loop_blocks_set, S, pred[j]);
		}
	}


}



void natloop::insert_on_stack(std::set<int> & loop_blocks_set, std::stack<int> & S, int a)
{
	if (loop_blocks_set.find(a) == loop_blocks_set.end())
	{
		loop_blocks_set.insert(a);
		loop_blocks.push_back(a);
		S.push(a);
	}

}



void natloop::print_natloop()
{
	// print the whole natrual loop as required format
	cout << "natloop " << loop_number << endl;
	print_backedge();
	print_preheader();
	print_loop_blocks();

}



void natloop::print_backedge()
{
	// print backedges head following list of tails
	int head_bb_number = head->get_bb_number();
	int_type tails_bb_number;

	int tails_size = tails.size();
	int i;
	for (i = 0; i < tails_size; i++)
		tails_bb_number.push_back(tails[i]->get_bb_number());

	std::sort(tails_bb_number.begin(), tails_bb_number.end());

	cout << "\t" << "backedge " << head_bb_number;

	for (i = 0; i < tails_size; i++)
	{
		cout << " " << tails_bb_number[i];
	} 
	cout << endl;
}



void natloop::print_preheader()
{
	// print preheader number
	int head_bb_number = head->get_bb_number();
	int preheader = head_bb_number - 1;

	cout << "\tpreheader " <<  preheader << endl;
}



void natloop::print_loop_blocks()
{
	// print bb_number of all nodes constituting this natloop
	std::sort(loop_blocks.begin(), loop_blocks.end());

	int loop_blocks_size = loop_blocks.size();

	cout << "\tloop_blocks";

	int i;
	for (i = 0; i < loop_blocks_size; i++)
	{
		cout << " " << loop_blocks[i];
	}
	cout << endl;
}
