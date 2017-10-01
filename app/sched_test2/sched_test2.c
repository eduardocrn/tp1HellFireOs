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

void aperiodic(void) {
	uint32_t r, id;

	
	id = hf_selfid();

	printf("\n APERIODIC task ID: %d", id);

	while (1) {
		r = random();
		if (r >= 50 && r <= 500)
			break;
	}

	delay_ms(r);
}

void aperiodic_task_create(void) {

	int32_t i, id;

	id = hf_selfid();

	printf("\n%s (%d)[%d][%d]", hf_selfname(), id, hf_jobs(id), hf_dlm(id));

	for (i = 0; i < 3; i++) {
		hf_spawn(aperiodic, 0, 1, 0, "task aperiodic", 2048);
	}

}

void app_main(void){
	hf_spawn(task, 4, 1, 4, "task a", 2048);
	hf_spawn(task, 8, 2, 8, "task b", 2048);
	hf_spawn(task, 12, 3, 12, "task c", 2048);

	hf_spawn(polling_server, 3, 1, 3, "polling server", 1024);

	hf_spawn(aperiodic_task_create, 0, 0, 0, "task create aperiodic", 1024);
}
