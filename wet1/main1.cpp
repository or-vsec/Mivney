/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2016-2017                                     */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"
#include <iostream>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
	NONE_CMD = -2,
	COMMENT_CMD = -1,
	INIT_CMD = 0,
	ADDSTUDENT_CMD = 1,
	ADDTEAM_CMD = 2,
	MOVESTUDENTTOTEAM_CMD = 3,
	GETMOSTPOWERFUL_CMD = 4,
	REMOVESTUDENT_CMD = 5,
	GETALLSTUDENTS_CMD = 6,
	INCREASELEVEL_CMD = 7,
	QUIT_CMD = 8
} commandType;

static const int numActions = 9;
static const char *commandStr[] = {
		"Init",
		"AddStudent",
		"AddTeam",
		"MoveStudentToTeam",
		"GetMostPowerful",
		"RemoveStudent",
		"GetAllStudentsByPower",
		"IncreaseLevel",
		"Quit" };

static const char* ReturnValToStr(int val) {
	switch (val) {
	case SUCCESS:
		return "SUCCESS";
	case ALLOCATION_ERROR:
		return "ALLOCATION_ERROR";
	case FAILURE:
		return "FAILURE";
	case INVALID_INPUT:
		return "INVALID_INPUT";
	default:
		return "";
	}
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
	error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {
	char buffer[MAX_STRING_INPUT_SIZE];

	// Reading commands
	while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
		fflush(stdout);
		if (parser(buffer) == error)
			break;
	};
	return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
		const char** const command_arg) {
	if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
		return (NONE_CMD);
	if (StrCmp("#", command)) {
		if (strlen(command) > 1)
			printf("%s", command);
		return (COMMENT_CMD);
	};
	for (int index = 0; index < numActions; index++) {
		if (StrCmp(commandStr[index], command)) {
			*command_arg = command + strlen(commandStr[index]) + 1;
			return ((commandType) index);
		};
	};
	return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddStudent(void* DS, const char* const command);
static errorType OnAddTeam(void* DS, const char* const command);
static errorType OnMoveStudentToTeam(void* DS, const char* const command);
static errorType OnRemoveStudent(void* DS, const char* const command);
static errorType OnIncreaseLevel(void* DS, const char* const command);
static errorType OnGetMostPowerful(void* DS, const char* const command);
static errorType OnGetAllStudentsByPower(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
	static void *DS = NULL; /* The general data structure */
	const char* command_args = NULL;
	errorType rtn_val = error;

	commandType command_val = CheckCommand(command, &command_args);

	switch (command_val) {

	case (INIT_CMD):
		rtn_val = OnInit(&DS, command_args);
		break;
	case (ADDSTUDENT_CMD):
		rtn_val = OnAddStudent(DS, command_args);
		break;
	case (ADDTEAM_CMD):
		rtn_val = OnAddTeam(DS, command_args);
		break;
	case (MOVESTUDENTTOTEAM_CMD):
		rtn_val = OnMoveStudentToTeam(DS, command_args);
		break;
	case (REMOVESTUDENT_CMD):
		rtn_val = OnRemoveStudent(DS, command_args);
		break;
	case (INCREASELEVEL_CMD):
		rtn_val = OnIncreaseLevel(DS, command_args);
		break;
	case (GETMOSTPOWERFUL_CMD):
		rtn_val = OnGetMostPowerful(DS, command_args);
		break;
	case (GETALLSTUDENTS_CMD):
		rtn_val = OnGetAllStudentsByPower(DS, command_args);
		break;
	case (QUIT_CMD):
		rtn_val = OnQuit(&DS, command_args);
		break;

	case (COMMENT_CMD):
		rtn_val = error_free;
		break;
	case (NONE_CMD):
		rtn_val = error;
		break;
	default:
		assert(false);
		break;
	};
	return (rtn_val);
}

/***************************************************************************/
/* OnInit                                                                  */
/***************************************************************************/
static errorType OnInit(void** DS, const char* const command) {
	if (isInit) {
		printf("Init was already called.\n");
		return (error_free);
	};
	isInit = true;

	*DS = Init();
	if (*DS == NULL) {
		printf("Init failed.\n");
		return error;
	};
	printf("Init done.\n");

	return error_free;
}


/***************************************************************************/
/* OnAddStudent                                                          */
/***************************************************************************/
static errorType OnAddStudent(void* DS, const char* const command) {
	int studentID;
	int Grade;
	int power;
	ValidateRead(
			sscanf(command, "%d %d %d", &studentID, &Grade, &power),
			3, "AddStudent failed.\n");
	StatusType res = AddStudent(DS, studentID, Grade, power);

	if (res != SUCCESS) {
		printf("AddStudent: %s\n", ReturnValToStr(res));
		return error_free;
	}

	printf("AddStudent: %s\n", ReturnValToStr(res));
	return error_free;
}

/***************************************************************************/
/* OnAddTeam                                                             */
/***************************************************************************/
static errorType OnAddTeam(void* DS, const char* const command) {
	int magiID;
	ValidateRead(sscanf(command, "%d", &magiID), 1, "AddTeam failed.\n");
	StatusType res = AddTeam(DS, magiID);

	if (res != SUCCESS) {
		printf("AddTeam: %s\n", ReturnValToStr(res));
		return error_free;
	} else {
		printf("AddTeam: %s\n", ReturnValToStr(res));
	}

	return error_free;
}


/***************************************************************************/
/* OnAddStudentToTeam                                                            */
/***************************************************************************/
static errorType OnMoveStudentToTeam(void* DS, const char* const command) {
	int studentID, teamID;
	ValidateRead(sscanf(command, "%d %d", &studentID, &teamID), 2,
			"MoveStudentToTeam failed.\n");
	StatusType res = MoveStudentToTeam(DS, studentID, teamID);
	if (res != SUCCESS) {
		printf("MoveStudentToTeam: %s\n", ReturnValToStr(res));
		return error_free;
	}

	printf("MoveStudentToTeam: %s\n", ReturnValToStr(res));
	return error_free;
}

/***************************************************************************/
/* OnRemoveStudent                                                            */
/***************************************************************************/
static errorType OnRemoveStudent(void* DS, const char* const command) {
	int studentID;
	ValidateRead(sscanf(command, "%d", &studentID), 1,
			"RemoveStudent failed.\n");
	StatusType res = RemoveStudent(DS, studentID);

	if (res != SUCCESS) {
		printf("RemoveStudent: %s\n", ReturnValToStr(res));
		return error_free;
	}

	printf("RemoveStudent: %s\n", ReturnValToStr(res));
	return error_free;
}

/***************************************************************************/
/* OnIncreaseLevel                                                         */
/***************************************************************************/
static errorType OnIncreaseLevel(void* DS, const char* const command) {
	int grade;
	int powerIncrease;
	ValidateRead(sscanf(command, "%d %d", &grade, &powerIncrease), 2,
			"IncreaseLevel failed.\n");
	StatusType res = IncreaseLevel(DS, grade, powerIncrease);

	if (res != SUCCESS) {
		printf("IncreaseLevel: %s\n", ReturnValToStr(res));
		return error_free;
	}

	printf("IncreaseLevel: %s\n", ReturnValToStr(res));
	return error_free;
}


/***************************************************************************/
/* OnGetMostPowerful                                                         */
/***************************************************************************/
static errorType OnGetMostPowerful(void* DS, const char* const command) {
	int teamID;
	ValidateRead(sscanf(command, "%d", &teamID), 1, "GetMostPowerful failed.\n");
	int studentID;
	StatusType res = GetMostPowerful(DS, teamID, &studentID);

	if (res != SUCCESS) {
		printf("GetMostPowerful: %s\n", ReturnValToStr(res));
		return error_free;
	}

	cout << "Most powerful student is: " << studentID << endl;
	return error_free;
}

/***************************************************************************/
/* PrintAll                                                        */
/***************************************************************************/

void PrintAll(int *studentIDs, int numOfStudents) {
	if (numOfStudents > 0) {
		cout << "Rank\t||\tStudent" << endl;
	}

	for (int i = 0; i < numOfStudents; i++) {
		cout << i + 1 << "\t\t||\t" << studentIDs[i] << endl;
	}
	cout << "and there are no more students!" << endl;

	free (studentIDs);
}

static errorType OnGetAllStudentsByPower(void* DS, const char* const command) {
	int teamID;
	ValidateRead(sscanf(command, "%d", &teamID), 1,
			"GetAllStudentsByPower failed.\n");
	int* studentIDs;
	int numOfStudents;
	StatusType res = GetAllStudentsByPower(DS, teamID, &studentIDs, &numOfStudents);

	if (res != SUCCESS) {
		printf("GetAllStudentsByPower: %s\n", ReturnValToStr(res));
		return error_free;
	}

	PrintAll(studentIDs, numOfStudents);
	return error_free;
}

/***************************************************************************/
/* OnQuit                                                                  */
/***************************************************************************/
static errorType OnQuit(void** DS, const char* const command) {
	Quit(DS);
	if (*DS != NULL) {
		printf("Quit failed.\n");
		return error;
	};

	isInit = false;
	printf("Quit done.\n");

	return error_free;
}

#ifdef __cplusplus
}
#endif
