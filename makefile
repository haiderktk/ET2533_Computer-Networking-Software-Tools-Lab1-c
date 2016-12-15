MYFILES = task1 task2 task3 task4 task5_client task5_server task6_client task6_server

TOOLS: task1 task2 task3 task4 task5_client task5_server task6_client task6_server

task1:
	gcc task1.c -o task1

task2:: 
	gcc task2.c -o task2

task3:
	gcc task3.c -o task3

task4:
	gcc task4.c -o task4

task5_client:
	gcc task5_client.c -o task5_client

task5_server:
	gcc -pthread task5_server.c -o task5_server

task6_client:
	gcc task6_client.c -o task6_client

task6_server:
	gcc task6_server.c -o task6_server


clean:
	rm -rf $(MYFILES) 
		
