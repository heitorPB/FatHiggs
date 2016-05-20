#ifndef MYHIGGS_H_
#define MYHIGGS_H_ 1

#include "TTree.h"
#include <string>


namespace MyHiggs
{
	void analyze(std::string path, TTree *out_tree);
}


#endif
