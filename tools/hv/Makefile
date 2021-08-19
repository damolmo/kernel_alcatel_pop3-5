# Makefile for Hyper-V tools

CC = $(CROSS_COMPILE)gcc
PTHREAD_LIBS = -lpthread
WARNINGS = -Wall -Wextra
CFLAGS = $(WARNINGS) -g $(PTHREAD_LIBS)

<<<<<<< HEAD
all: hv_kvp_daemon hv_vss_daemon
=======
CFLAGS += -D__EXPORTED_HEADERS__ -I../../include/uapi -I../../include

all: hv_kvp_daemon hv_vss_daemon hv_fcopy_daemon
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
<<<<<<< HEAD
	$(RM) hv_kvp_daemon hv_vss_daemon
=======
	$(RM) hv_kvp_daemon hv_vss_daemon hv_fcopy_daemon
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
