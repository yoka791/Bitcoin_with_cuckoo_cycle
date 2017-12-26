working with vs 2017 linux
on linux machine:
1.	install ssh: 
	sudo apt-get install openssh-server openssh-client
2.	use static ip, set it.

-copy includes from linux to windows:
/usr/include

on vs:
1.	new project->cross platform->linux->console application/make file project.
2.	enter IP/user/password of VM.
3.	go to project properties
	a. Debugging->Program Arguments-> "-h 211"
	b. VC++ Directories->incluse directories->enter copied include folder.
	c. C/C++->Preprocesser-> enter: "SHOW;IDXSHIFT=0;PROOFSIZE=6;EDGEBITS=3;"
4.  	Right click on the project->add->existing item. add files you want.

Google Test:
1.	Right click on solution->Add->New Project->Visual c++->Test->Google Test.
2.	include the appropriate tested file.	
3. 	Test->Windows->Test explorer
4.	Write Tests.
5.  	on Test explorer: Run all tests.


    
