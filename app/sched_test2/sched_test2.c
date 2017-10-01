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
	hf_yield();
}

void aperiodic_task_create(void) {

	int32_t i, id;

	id = hf_selfid();

	printf("\n%s (%d)[%d][%d]", hf_selfname(), id, hf_jobs(id), hf_dlm(id));

	for (i = 0; i < 3; i++) {
		hf_spawn(aperiodic, 0, 1, 0, "task aperiodic", 2048);
	}

}

void polling_server() {
	printf("\n I'm a POLLING SERVER task");
	int32_t rc;
	int32_t k;
	volatile int32_t status;

	status = _di();
	#if KERNEL_LOG >= 1
			dprintf("hf_polling_server() %d ", (uint32_t)_read_us());
	#endif
		krnl_task = &krnl_tcb[krnl_current_task];
		printf("\n Krnl Task %d", hf_selfid());
		rc = setjmp(krnl_task->task_context);
		if (rc){
			_ei(status);
			printf("\n MORREU POOLING");
			return;
		}
		if (krnl_task->state == TASK_RUNNING)
			krnl_task->state = TASK_READY;
		if (krnl_task->pstack[0] != STACK_MAGIC)
			panic(PANIC_STACK_OVERFLOW);
		if (krnl_tasks > 0){
			//Size of queue
			k = aperiodic_sched();
			if (k == 0){
				hf_yield();
			}
			printf("\n Capacidade da fila  %d", hf_queue_count(krnl_ap_queue));

			if (krnl_task->capacity == 0)
				hf_kill(hf_selfid());
			else
				krnl_task->capacity--;

			krnl_task = hf_queue_remhead(krnl_ap_queue);
			printf("\n Current task: %d", krnl_task->id);
			printf("\n Capacidade da fila agora  %d", hf_queue_count(krnl_ap_queue));
			krnl_task->state = TASK_RUNNING;
			krnl_pcb.coop_cswitch++;
	#if KERNEL_LOG >= 1
			dprintf("\n%d %d %d %d %d ", krnl_current_task, krnl_task->period, krnl_task->capacity, krnl_task->deadline, (uint32_t)_read_us());
	#endif
			_restoreexec(krnl_task->task_context, status, krnl_current_task);
			panic(PANIC_UNKNOWN);
		}else{
			panic(PANIC_NO_TASKS_LEFT);
		}
		delay_ms(1);			// do not hog the CPU!
}


void app_main(void){
	hf_spawn(task, 4, 2, 4, "task periodic a", 2048);
	// hf_spawn(task, 3, 1, 3, "task periodic b", 2048);
	hf_spawn(task, 5, 3, 5, "task periodic c", 2048);
	hf_spawn(polling_server, 3, 1, 3, "polling server", 1024);

	hf_spawn(aperiodic_task_create, 0, 0, 0, "task create aperiodic", 1024);
}
