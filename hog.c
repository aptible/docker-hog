#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>

#define DEFAULT_CHECKPOINTS 64

static int term_requested = 0;

void sig_handler(int signo) {
	if (signo == SIGTERM) {
		term_requested = 1;
	}
}

int assign_env_pos_long(char* var_name, long* dst) {
	char* env_val = getenv(var_name);

	if (env_val == NULL) {
		return 0;
	}

	char* endptr = NULL;
	*dst = strtol(env_val, &endptr, 10);

	if ((*dst < 0) || (endptr == NULL) || (*endptr != 0)) {
		fprintf(stderr, "hog: invalid value for %s: %s\n",
			var_name, env_val);
		return 1;
	}

	return 0;
}


int main(int argc, char* argv[]) {
	if (signal(SIGTERM, sig_handler) == SIG_ERR) {
		perror("register SIGTERM handler");
		return 1;
	}

	long hog_max_bytes = LONG_MAX;
	if (assign_env_pos_long("HOG_MAX_BYTES", &hog_max_bytes)) {
		return 1;
	}

	long hog_wait_seconds = 0;
	if (assign_env_pos_long("HOG_WAIT_SECONDS", &hog_wait_seconds)) {
		return 1;
	}

	long hog_goal_seconds = 0;
	if (assign_env_pos_long("HOG_GOAL_SECONDS", &hog_goal_seconds)) {
		return 1;
	}

	long hog_checkpoints = DEFAULT_CHECKPOINTS;
	if (assign_env_pos_long("HOG_CHECKPOINTS", &hog_checkpoints)) {
		return 1;
	}

	fprintf(stdout, "hog: hog up to %ld bytes over %ld second(s) with %ld checkpoint(s)\n",
		hog_max_bytes, hog_goal_seconds, hog_checkpoints);
	fflush(stdout);

	if (hog_wait_seconds > 0) {
		fprintf(stdout, "hog: wait: %ld second(s)\n", hog_wait_seconds);
		fflush(stdout);
		sleep(hog_wait_seconds);
	}

	fprintf(stdout, "hog: start\n");
	fflush(stdout);

	long page_size = sysconf(_SC_PAGESIZE);
	long hogged_bytes = 0;
	long next_checkpoint_bytes = 0;

	while(1) {
		if (term_requested > 0) {
			fprintf(stdout, "hog: exit: SIGTERM\n");
			fflush(stdout);
			break;
		}

		if (hogged_bytes > hog_max_bytes) {
			fprintf(stdout, "hog: done\n");
			fflush(stdout);
			break;
		}

		if ((hog_checkpoints > 0) && (hogged_bytes > next_checkpoint_bytes)) {
			next_checkpoint_bytes += hog_max_bytes / hog_checkpoints;
			fprintf(stdout, "hog: checkpoint: %ld\n", hogged_bytes);
			fflush(stdout);
			if (hog_goal_seconds > 0) {
				usleep(1000 * 1000 * hog_goal_seconds / hog_checkpoints);
			}
		}

		char* range = (char*) malloc(page_size);
		if (range) {
			range[0] = 0;
		}

		hogged_bytes += page_size;
	}

	return 0;
}
