#include <hellfire.h>

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

	int32_t i, r;

	for (i = 0; i < 3; i++) {

		while (1) {
			r = random();
			if (r >= 50 && r <= 500)
				break;
		}

		hf_spawn(task, 0, delay_ms(r), 0, "task aperiodic", 2048);
	}

}

void app_main(void){
	hf_spawn(task, 4, 1, 4, "task periodic a", 2048);
	hf_spawn(task, 8, 2, 8, "task periodic b", 2048);
	hf_spawn(task, 12, 3, 12, "task periodic c", 2048);

	hf_spawn(aperiodic_task_create, 0, 0, 0, "tarefa criação aperiodicas", 2048);
}
