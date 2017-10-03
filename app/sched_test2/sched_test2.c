#include <hellfire.h>
#include <scheduler.h>

void task(void){
	int32_t jobs, id;
	
	id = hf_selfid();
	for(;;){
		jobs = hf_jobs(id);
		printf("\n%s (%d)[%d][%d]", hf_selfname(), id, hf_jobs(id), hf_dlm(id));
		while (jobs == hf_jobs(id));
	}
}

void aperiodic_task_create(void) {

	int32_t jobs, i, id;

	id = hf_selfid();

	for(;;) {
		jobs = hf_jobs(id);
		printf("\n%s (%d)[%d][%d]", hf_selfname(), id, hf_jobs(id), hf_dlm(id));
		hf_spawn(task, 0, 1, 0, "APERIODIC", 2048);
		delay_ms(random() % 100 + 50);
	}
}

void app_main(void){
	hf_spawn(task, 4, 1, 4, "task a", 2048);
	hf_spawn(task, 8, 2, 8, "task b", 2048);
	hf_spawn(task, 12, 3, 12, "task c", 2048);

	hf_spawn(polling_server, 3, 1, 3, "polling server", 1024);

	hf_spawn(aperiodic_task_create, 0, 0, 0, "create aperiodic", 1024);
}
