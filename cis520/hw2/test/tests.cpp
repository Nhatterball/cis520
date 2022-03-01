#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}

/*
* LOAD_PROCESS_CONTROL_BLOCKS UNIT TEST CASES
*/

TEST(load_process_control_blocks, NullFile){
    ASSERT_EQ(NULL, load_process_control_blocks(NULL));
}

TEST(load_process_control_blocks, BadFileName){
    const char* fn = "";
    dyn_array_t* dyn_array = load_process_control_blocks(fn);
    ASSERT_EQ(dyn_array, (dyn_array_t*)NULL);
    
}

/*
* FIRST_COME_FIRST_SERVE UNIT TEST CASES
* Reference memset: https://www.tutorialspoint.com/c_standard_library/c_function_memset.htm
*/
TEST(first_come_first_serve, NullSchedule){
    dyn_array_t* rq = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ASSERT_EQ(false, first_come_first_serve(rq, NULL));
}

TEST(first_come_first_serve, GoodInput)
{
    ScheduleResult_t* res = new ScheduleResult_t;
    dyn_array_t* dyn_array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    memset(res, 0, sizeof(ScheduleResult_t));
    
    ProcessControlBlock_t pcb[4] = //pcb queue
    {
        [0] = {15, 5, 0, 0},
        [1] = {8, 4, 0, 0},
        [2] = {6, 2, 0, 0},
        [3] = {3, 1, 0, 0}
    };
    
    dyn_array_push_back(dyn_array, &pcb[3]);//take from back
    dyn_array_push_back(dyn_array, &pcb[2]);
    dyn_array_push_back(dyn_array, &pcb[1]);
    dyn_array_push_back(dyn_array, &pcb[0]);
    ASSERT_EQ(true, first_come_first_serve(dyn_array, res));
    
}


/*
* SHORTEST_JOB_FIRST UNIT TEST CASES
*/

TEST(shortest_job_first, NullQ){
    ScheduleResult_t* res = new ScheduleResult_t;
    EXPECT_EQ(false, shortest_job_first(NULL, res));
}

TEST(shortest_job_first, GoodInput){
    ScheduleResult_t* res = new ScheduleResult_t;
    dyn_array_t* dyn_array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    memset(res, 0, sizeof(ScheduleResult_t));
    
    ProcessControlBlock_t pcb[4] = //pcb queue
    {
        [0] = {15, 5, 0, 0},
        [1] = {8, 4, 1, 0},
        [2] = {6, 2, 2, 0},
        [3] = {3, 1, 3, 0}
    };
    
    dyn_array_push_back(dyn_array, &pcb[3]);//take from back
    dyn_array_push_back(dyn_array, &pcb[2]);
    dyn_array_push_back(dyn_array, &pcb[1]);
    dyn_array_push_back(dyn_array, &pcb[0]);
    ASSERT_EQ(true, shortest_job_first(dyn_array, res));
    
}


/*
* ROUND_ROBIN UNIT TEST CASES
*/

TEST(round_robin, NullSchedule){
    dyn_array_t* dyn_array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ASSERT_EQ(false, round_robin(dyn_array, NULL, QUANTUM));
}

TEST(round_robin, GoodInput){
    ScheduleResult_t* res = new ScheduleResult_t;
    dyn_array_t* dyn_array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    memset(res, 0, sizeof(ScheduleResult_t));
    
    ProcessControlBlock_t pcb[4] = //pcb queue
    {
        [0] = {15, 5, 0, 0},
        [1] = {8, 4, 0, 0},
        [2] = {6, 2, 0, 0},
        [3] = {3, 1, 0, 0}
    };
    
    dyn_array_push_back(dyn_array, &pcb[3]);//take from back
    dyn_array_push_back(dyn_array, &pcb[2]);
    dyn_array_push_back(dyn_array, &pcb[1]);
    dyn_array_push_back(dyn_array, &pcb[0]);
    ASSERT_EQ(true, round_robin(dyn_array, res, QUANTUM));
    
}

/*
* SHORTEST_REMAINING_TIME_FIRST UNIT TEST CASES
*/

TEST(shortest_remaining_time_first, NullSchedule){
    dyn_array_t* dyn_array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ASSERT_EQ(false, shortest_remaining_time_first(dyn_array, NULL));
}

TEST(shortest_remaining_time_first, GoodInput){
    ScheduleResult_t* res = new ScheduleResult_t;
    dyn_array_t* dyn_array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    memset(res, 0, sizeof(ScheduleResult_t));
    
    ProcessControlBlock_t pcb[4] = //pcb queue
    {
        [0] = {15, 5, 0, 0},
        [1] = {8, 4, 1, 0},
        [2] = {6, 2, 2, 0},
        [3] = {3, 1, 3, 0}
    };
    
    dyn_array_push_back(dyn_array, &pcb[3]);//take from back
    dyn_array_push_back(dyn_array, &pcb[2]);
    dyn_array_push_back(dyn_array, &pcb[1]);
    dyn_array_push_back(dyn_array, &pcb[0]);
    ASSERT_EQ(true, shortest_remaining_time_first(dyn_array, res));
    
}
















