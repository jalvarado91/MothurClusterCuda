#include "mothur.h"
#include "mothurout.h"
#include "command.hpp"

MothurOut* MothurOut::_uniqueInstance = 0;

void ctrlc_handler ( int sig ) {
	MothurOut* m = MothurOut::getInstance();
    ctrlc_pressed = 1;
	m->control_pressed = ctrlc_pressed;
	
	if (m->executing) { //if mid command quit execution, else quit mothur
		m->mothurOutEndLine(); m->mothurOut("quitting command...");  m->mothurOutEndLine();
	}else{
		m->mothurOut("quitting mothur");  m->mothurOutEndLine();
		exit(1);
	}
}
int main(int argc, char *argv[]){
	MothurOut* m = MothurOut::getInstance();

	string optionString = "column=sogin.unique.filter.dist, names=sogin.names";
	Command* command = new ClusterCommand(optionString);	

	int quitCommandCalled = 0;
	quitCommandCalled = command->execute();
}