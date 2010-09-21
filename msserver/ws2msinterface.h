
#ifndef _WS2MSINTERFACE_H_
#define _WS2MSINTERFACE_H_

#define CMD_START_DATA_COLLECTION 	0
#define CMD_RESTART_SERVER			1
#define CMD_SHUTDOWN_SERVER			2	
#define CMD_RUN_SHELL_SCRIPT		3

typedef struct _WSCmd
{
	char cmd;
	char numArgs;
	void *args;
} WSCmd;

typedef struct _DataCollectionArgs
{
	char interval; //in seconds
	char* serverNames;
} DataCollectionArgs;

typedef struct _RestartServerArgs
{
	char* serverNames;
} RestartServerArgs;

typedef struct _ShutdownServerArgs
{
	char* serverNames;
} ShutdownServerArgs;

typedef struct _RunShellScriptArgs
{
	int shellScriptSize;
	char* shellScript;
	char* serverNames;
} RunShellScriptArgs;

#endif
