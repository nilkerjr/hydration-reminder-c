#include <stdio.h>
#include <unistd.h>  // for sleep()
#include <stdlib.h>
#include <windows.h>  // for SetConsoleOutputCP

void displayProgress(int waterIntake, int dailyGoal) 
{
    int i = 0;
    int cupSize = 250; // 1 💧 represents 250 ml
    int totalCups = 0;
    int filledCups = 0; 
   
    totalCups = (dailyGoal + cupSize - 1) / cupSize;
    filledCups = (waterIntake + cupSize - 1) / cupSize;
    
    printf("Water Intake: ");
    for (i = 0; i < filledCups; i++)
    {
        printf("💧");
    }

    for (i = filledCups; i < totalCups; i++)
    {
        // printf("");
    }

    printf(" (%d/%d ml)\n", waterIntake, dailyGoal);
}


int main() 
{
    int dailyGoal = 0;
    int waterIntake = 0;
    int amount = 0;
    
    SetConsoleOutputCP(CP_UTF8);
    printf("Welcome to Hydration Reminder (C version)!\n");
    printf("Enter your daily water goal in ml: ");
    scanf("%d", &dailyGoal);

    while (1) 
    { 
        // infinite loop
        if (waterIntake >= dailyGoal) 
        {
            printf("\n🎉 You reached your daily goal! Keep tracking if you want more water.\n");
        }

        printf("\nEnter amount of water you drank (ml) or 0 to skip: ");
        scanf("%d", &amount);

        if (amount > 0) 
        {
            waterIntake += amount;
            displayProgress(waterIntake, dailyGoal);
        }

        printf("Next reminder in 30 minutes.\n");
        sleep(1); // wait 30 minutes
    }

    return 0;
}