#ifndef SERVERSHRP_H
#define SERVERSHRP_H
#include <config.h>
#ifdef HAVE_WINDOWS_H
#include <windows.h>
typedef void *pthread_t;
extern int pthread_lock_global_np();
extern int pthread_unlock_global_np();
extern int pthread_cond_signal();
extern int pthread_cancel();
extern int pthread_mutex_lock();
extern int pthread_mutex_unlock();
extern int pthread_mutex_init();
extern int pthread_cond_init();
extern int pthread_cond_wait();
extern int pthread_create();
extern int pthread_cleanup_push();
extern int pthread_cleanup_pop();
extern int pthread_detach();
extern BOOL pthread_mutex_destroy();
extern BOOL pthread_cond_destroy();
#define close closesocket
#else
#include <pthread.h>
#ifndef HAVE_PTHREAD_LOCK_GLOBAL_NP
extern void pthread_lock_global_np();
extern void pthread_unlock_global_np();
#endif
#endif

#define SrvNoop        0           /**** Used to start server ****/
#define SrvAbort       1           /**** Abort current action or mdsdcl command ***/
#define SrvAction      2           /**** Execute an action nid in a tree ***/
#define SrvClose       3           /**** Close open trees ***/
#define SrvCreatePulse 4           /**** Create pulse files for single tree (no subtrees) ***/
#define SrvSetLogging  5           /**** Turn logging on/off ***/
#define SrvCommand     6           /**** Execute MDSDCL command ***/
#define SrvMonitor     7           /**** Broadcast messages to action monitors ***/
#define SrvShow        8           /**** Request current status of server ***/
#define SrvStop        9           /**** Stop server ***/
#define SrvRemoveLast  10          /**** Remove last entry in the queue if jobs pending ***/

#define MonitorBuildBegin 1
#define MonitorBuild      2
#define MonitorBuildEnd   3
#define MonitorCheckin    4
#define MonitorDispatched 5
#define MonitorDoing      6
#define MonitorDone       7

#define SrvJobABORTED     1
#define SrvJobSTARTING    2
#define SrvJobFINISHED    3
#define SrvJobCHECKEDIN   4

#define SrvJobBEFORE_NOTIFY 1
#define SrvJobAFTER_NOTIFY  2

#define ServerABORT    0xfe18032

typedef struct { struct _SrvJob *next;
                 struct _SrvJob *previous;
                 int addr;
                 int port;
                 int sock;
                 int length;
                 int op;
                 int flags;
                 int jobid;
               } JHeader;
                 
typedef struct _SrvJob { JHeader h;
               } SrvJob; 

typedef struct { JHeader h;
                 char    *tree;
                 int     shot;
                 int     nid;
               } SrvActionJob;

typedef struct { JHeader h;
               } SrvCloseJob;

typedef struct { JHeader h;
                 char    *tree;
                 int     shot;
               } SrvCreatePulseJob;

typedef struct { JHeader h;
                 char    *table;
                 char    *command;
               } SrvCommandJob;

typedef struct { JHeader h;
                 char    *tree;
                 int     shot;
                 int     phase;
                 int     nid;
                 int     on;
                 int     mode;
                 char    *server;
                 int     status;
               } SrvMonitorJob;

typedef struct { char server[32];
                 int  nid;
                 int  phase;
                 int  sequence;
                 int  num_references;
                 int  *referenced_by;
                 struct descriptor *condition;
                 int  status;
                 int  netid;
                 unsigned on : 1;
                 unsigned done : 1;
                 unsigned closed : 1;
                 unsigned dispatched : 1;
		 unsigned doing : 1;
		 unsigned recorded : 1;
                 char *path;
                 char *event;
               } ActionInfo;

typedef struct { int num;
                 char tree[13];
                 int shot;
                 int failed_essential;
                 ActionInfo actions[1];
               } DispatchTable;

typedef struct { int shot;
                 union {
                     int nid;
                     int phase;
                 } u;
                 int status;
               } DispatchEvent;

#ifndef _NO_SERVER_SEND_MESSAGE_PROTO
extern int ServerSendMessage( int *msgid, char *server, int op, int *retstatus, int *socket,
                         void (*ast)(), void *astparam, void (*before_ast)(),
  int numargs_in, ...);
#endif
extern int ServerConnect(char *);
#endif
