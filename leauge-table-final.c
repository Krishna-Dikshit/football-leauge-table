#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int new_id = 1;

struct node {
    struct node *next;
    struct node *prev;
    int Sgoals, Cgoals, games, points, win, loss, draw, id;
    char name[30];
};

struct node *start = NULL;

struct node *addteam(struct node *);
struct node *removeteam(struct node *);
struct node *checkteam(struct node *);
struct node *display(struct node *);
struct node *addgameweek(struct node *);
void bubbleSortDescending(int arr[], int n);

int main() {
    int opt;
    do {
        printf("MENU\n1. Add team\n2. Remove team\n3. Check team\n4. Display\n5. Add game week\n0. Exit\nEnter your choice: ");
        scanf("%d", &opt);
        switch (opt) {
            case 0:
                printf("\nExiting.....\n");
                break;
            case 1:
                start = addteam(start);
                break;
            case 2:
                start = removeteam(start);
                break;
            case 3:
                start = checkteam(start);
                break;
            case 4:
                start = display(start);
                break;
            case 5:
                start = addgameweek(start);
                break;
            default:
                printf("Enter a valid option\n");
                break;
        }
    } while (opt != 0);
    return 0;
}

// Function to add a team
struct node *addteam(struct node *start) {
    struct node *newteam, *ptr;
    newteam = (struct node *)malloc(sizeof(struct node));
    newteam->Cgoals = 0;
    newteam->Sgoals = 0;
    newteam->win = 0;
    newteam->draw = 0;
    newteam->loss = 0;
    newteam->points = 0;
    newteam->games=0;
    newteam->id = new_id++;

    printf("\nEnter team name: ");
    scanf("%s", newteam->name);
    
    if (!start) {  // If the list is empty
        start = newteam;
        newteam->next = NULL;
        newteam->prev = NULL;
    } else {
        ptr = start;
        while (ptr->next) {
            ptr = ptr->next;
        }
        ptr->next = newteam;
        newteam->prev = ptr;
        newteam->next = NULL;
    }

    return start;
}

// Function to remove a team
struct node *removeteam(struct node *start) {
    struct node *ptr = start;
    char name[30];
    int found = 0;

    printf("Enter the name of the team to remove: ");
    scanf("%s", name);

    while (ptr != NULL) {
        if (strcmp(ptr->name, name) == 0) {
            found = 1;

            if (ptr->prev == NULL) {  // First node
                start = ptr->next;
                if (start) {
                    start->prev = NULL;
                }
            } else {
                ptr->prev->next = ptr->next;
                if (ptr->next) {
                    ptr->next->prev = ptr->prev;
                }
            }

            free(ptr);
            printf("| Team removed |\n");
            break;
        }
        ptr = ptr->next;
    }

    if (!found) {
        printf("Team not found\n");
    }

    return start;
}

// Function to check a team's details
struct node *checkteam(struct node *start) {
    struct node *ptr = start;
    char name[30];
    int found = 0;

    printf("Enter the name of the team: ");
    scanf("%s", name);

    while (ptr != NULL) {
        if (strcmp(ptr->name, name) == 0) {
            printf("___________\n");
            printf("Team name: %s\n", ptr->name);
            printf("Goals scored: %d\n", ptr->Sgoals);
            printf("Goals conceded: %d\n", ptr->Cgoals);
            printf("Wins: %d\tDraws: %d\tLosses: %d\n", ptr->win, ptr->draw, ptr->loss);
            printf("Points: %d\n", ptr->points);
            return start;
        }
        ptr = ptr->next;
    }

    printf("Team not found!\n");
    return start;
}

// Function to display the league table
struct node* display(struct node* start) {
    if (start == NULL) {
        printf("No teams to display.\n");
        return start;
    }

    struct node* ptr = start;
    int max = 0;

    // Count the number of teams
    while (ptr != NULL) {
        max++;
        ptr = ptr->next;
    }

    // Create an array to store pointers to teams (nodes)
    struct node* teams[max];
    ptr = start;
    int i = 0;

    // Populate the teams array with pointers to the nodes
    while (ptr != NULL) {
        teams[i] = ptr;
        i++;
        ptr = ptr->next;
    }

    // Sort the teams array by points, games played, and goal difference
    for (i = 0; i < max - 1; i++) {
        for (int j = i + 1; j < max; j++) {
            // Compare first by points
            if (teams[i]->points < teams[j]->points) {
                struct node* temp = teams[i];
                teams[i] = teams[j];
                teams[j] = temp;
            }
            // If points are equal, compare by games played
            else if (teams[i]->points == teams[j]->points) {
                if (teams[i]->games > teams[j]->games) {
                    struct node* temp = teams[i];
                    teams[i] = teams[j];
                    teams[j] = temp;
                }
                // If games played are equal, compare by goal difference
                else if (teams[i]->games == teams[j]->games) {
                    int goal_diff_i = teams[i]->Sgoals - teams[i]->Cgoals;
                    int goal_diff_j = teams[j]->Sgoals - teams[j]->Cgoals;

                    if (goal_diff_i < goal_diff_j) {
                        struct node* temp = teams[i];
                        teams[i] = teams[j];
                        teams[j] = temp;
                    }
                }
            }
        }
    }

    // Display the sorted table
    printf("Team name Matches-played Wins Draws Losses Goals-scored Goals-conceded   Points\n");
    for (i = 0; i < max; i++) {
        struct node*team = teams[i];
        printf("%d. %s  \t %d\t %d\t %d\t %d\t  %d\t  %d\t     %d\n", i, team->name, team->games, team->win, team->draw, team->loss, team->Sgoals, team->Cgoals, team->points);
    }

    return start;
}
// Function to add a game week
struct node *addgameweek(struct node *start) {
    if (!start || !start->next) {
        printf("Not enough teams to play a game.\n");
        return start;
    }

    int team1_index, team2_index, team1_goals, team2_goals;
    struct node *team1 = NULL, *team2 = NULL, *temp = start;
    int index = 1;

    // Display available teams
    printf("Available Teams:\n");
    while (temp != NULL) {
        printf("%d. %s\nGames played: %d\n\n", index++, temp->name, temp->games);
        temp = temp->next;
    }

    // Get team selections
    printf("Enter index of Team 1: ");
    scanf("%d", &team1_index);
    printf("Enter index of Team 2: ");
    scanf("%d", &team2_index);

    if (team1_index == team2_index) {
        printf("Teams must be different.\n");
        return start;
    }

    // Traverse to selected teams
    temp = start;
    index = 1;
    while (temp != NULL) {
        if (index == team1_index) team1 = temp;
        if (index == team2_index) team2 = temp;
        temp = temp->next;
        index++;
    }

    if (!team1 || !team2) {
        printf("Invalid teams.\n");
        return start;
    }

    // Get match results
    printf("Enter goals scored by Team %d: ", team1_index);
    scanf("%d", &team1_goals);
    printf("Enter goals scored by Team %d: ", team2_index);
    scanf("%d", &team2_goals);

    // Update stats for both teams
    team1->games++;
    team1->Sgoals += team1_goals;
    team1->Cgoals += team2_goals;

    team2->games++;
    team2->Sgoals += team2_goals;
    team2->Cgoals += team1_goals;

    // Determine match result and update points
    if (team1_goals > team2_goals) {
        team1->win++;
        team1->points += 3;
        team2->loss++;
    } else if (team1_goals == team2_goals) {
        team1->draw++;
        team2->draw++;
        team1->points++;
        team2->points++;
    } else {
        team2->win++;
        team2->points += 3;
        team1->loss++;
    }

    printf("Game week updated successfully.\n");
    return start;
}

// Bubble sort function for descending order
void bubbleSortDescending(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}