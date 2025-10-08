#include <stdio.h>
#include <unistd.h>  // for sleep()
#include <stdlib.h>
#include <windows.h>  // for SetConsoleOutputCP

void displayProgress(int waterIntake, int dailyGoal) 
{
    int progressUnits = (waterIntake * 20) / dailyGoal; // 20-unit bar
    int i = 0;

    printf("[");
    
    for (i = 0; i < progressUnits; i++)
    {
        printf("#");
    }

    for (i = progressUnits; i < 20; i++) {
        printf("-");
    }

    printf("] %d%%\n", (waterIntake * 100) / dailyGoal);
}

int main() 
{
    int dailyGoal = 0;
    int waterIntake = 0;
    int amount = 0;

    SetConsoleOutputCP(CP_UTF8);  // enable UTF-8
    printf("💧 Welcome to Hydration Reminder (C version)!\n");
    printf("Enter your daily water goal in ml: ");
    scanf("%d", &dailyGoal);

    while (waterIntake < dailyGoal)
    {
        printf("\nEnter amount of water you drank (ml): ");
        scanf("%d", &amount);

        waterIntake += amount;
        if (waterIntake > dailyGoal)
        {
            waterIntake = dailyGoal;
        }

        displayProgress(waterIntake, dailyGoal);

        if (waterIntake < dailyGoal) 
        {
            printf("Keep going! Next reminder in 30 minutes.\n");
            sleep(1800);  // wait 30 minutes (1800 seconds)
        } 
        else 
        {
            printf("🎉 Congratulations! You reached your daily goal.\n");
        }
    }

    return 0;
}