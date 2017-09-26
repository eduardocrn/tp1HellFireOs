#include <hellfire.h>
#include <random.h>

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

	int32_t i;

	for(i=0; i < 3; i++) {
		
	}

}

void app_main(void){
	hf_spawn(task, 4, 1, 4, "task a", 2048);
	hf_spawn(task, 8, 2, 8, "task b", 2048);
	hf_spawn(task, 12, 3, 12, "task c", 2048);

	hf_spawn(aperiodic_task_create, 0, 10, 15, "tarefa criação aperiodicas", 2048);
}
