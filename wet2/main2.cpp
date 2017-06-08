/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2016-2017                                     */
/* Homework : Wet 2                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main2.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library2.h"
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
    REMOVESTUDENT_CMD = 2,
    JOINTEAMS_CMD = 3,
    TEAMFIGHT_CMD = 4,
    GETNUMOFWINS_CMD = 5,
    GETLEADER_CMD = 6,
    QUIT_CMD = 7
} commandType;

static const int numActions = 8;
static const char *commandStr[] = {
        "Init",
        "AddStudent",
        "RemoveStudent",
        "JoinTeams",
        "TeamFight",
        "GetNumOfWins",
        "GetStudentLeader",
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
static errorType OnRemoveStudent(void* DS, const char* const command);
static errorType OnJoinTeams(void* DS, const char* const command);
static errorType OnTeamFight(void* DS, const char* const command);
static errorType OnGetNumOfWins(void* DS, const char* const command);
static errorType OnGetStudentLeader(void* DS, const char* const command);
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
        case (REMOVESTUDENT_CMD):
            rtn_val = OnRemoveStudent(DS, command_args);
            break;
        case (JOINTEAMS_CMD):
            rtn_val = OnJoinTeams(DS, command_args);
            break;
        case (TEAMFIGHT_CMD):
            rtn_val = OnTeamFight(DS, command_args);
            break;
        case (GETNUMOFWINS_CMD):
            rtn_val = OnGetNumOfWins(DS, command_args);
            break;
        case (GETLEADER_CMD):
            rtn_val = OnGetStudentLeader(DS, command_args);
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
    int n;
    ValidateRead(sscanf(command, "%d", &n),
     1, "Init failed.\n");
    *DS = Init(n);
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
    int Team;
    int power;
    ValidateRead(sscanf(command, "%d %d %d", &studentID, &Team, &power),
                 3, "AddStudent failed.\n");
    StatusType res = AddStudent(DS, studentID, Team, power);

    if (res != SUCCESS) {
        printf("AddStudent: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("AddStudent: %s\n", ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnRemoveStudent                                                             */
/***************************************************************************/
static errorType OnRemoveStudent(void* DS, const char* const command) {
    int studentID;
    ValidateRead(sscanf(command, "%d", &studentID), 1, "RemoveStudent failed.\n");
    StatusType res = RemoveStudent(DS, studentID);

    if (res != SUCCESS) {
        printf("RemoveStudent: %s\n", ReturnValToStr(res));
        return error_free;
    } else {
        printf("RemoveStudent: %s\n", ReturnValToStr(res));
    }

    return error_free;
}


/***************************************************************************/
/* OnJoinTeams                                                            */
/***************************************************************************/
static errorType OnJoinTeams(void* DS, const char* const command) {
    int team1,team2;
    ValidateRead(sscanf(command, "%d %d", &team1, &team2), 2,
                 "JoinTeams failed.\n");
    StatusType res = JoinTeams(DS, team1, team2);
    if (res != SUCCESS) {
        printf("JoinTeams: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("JoinTeams: %s\n", ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnTeamFight                                                            */
/***************************************************************************/
static errorType OnTeamFight(void* DS, const char* const command) {
    int team1,team2,numOfFighters;
    ValidateRead(sscanf(command, "%d %d %d", &team1, &team2, &numOfFighters), 3,
                 "TeamFight failed.\n");
    StatusType res = TeamFight(DS, team1,team2,numOfFighters);

    if (res != SUCCESS) {
        printf("TeamFight: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("TeamFight: %s\n", ReturnValToStr(res));
    return error_free;
}


/***************************************************************************/
/* OnGetNumOfWins                                                         */
/***************************************************************************/
static errorType OnGetNumOfWins(void* DS, const char* const command) {
    int teamID;
    ValidateRead(sscanf(command, "%d", &teamID), 1, "GetNumOfWins failed.\n");
    int wins;
    StatusType res = GetNumOfWins(DS, teamID, &wins);

    if (res != SUCCESS) {
        printf("GetNumOfWins: %s\n", ReturnValToStr(res));
        return error_free;
    }

    cout << "Team " << teamID << " won " << wins << " times." << endl;
    return error_free;
}


/***************************************************************************/
/* OnGetStudentLeader                                                         */
/***************************************************************************/
static errorType OnGetStudentLeader(void* DS, const char* const command) {
    int studentID;
    ValidateRead(sscanf(command, "%d", &studentID), 1, "GetStudentLeader failed.\n");
    int leader;
    StatusType res = GetStudentTeamLeader(DS, studentID, &leader);

    if (res != SUCCESS) {
        printf("GetStudentLeader: %s\n", ReturnValToStr(res));
        return error_free;
    }

    cout << "The leader is: " << leader << endl;
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