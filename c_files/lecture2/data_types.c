#include <stdio.h>

struct ask_questioins{
    char *text;
    char options[4][50];
    char answer;
}

int question(struct ask_question q){
    
    char ans;
    int attempt = 0;
    do{
        printf("/n%s/n", q.text);
        for(int i=0; i<4;i++)
        {
            printf("%c) \n%s\n", "A" + i, q.option[i]);
        }
        printf("Your answer: ")
        scanf("%c",&ans);
        ans = toupper(ans);
        if (ans != q.answer)
        {
            printf("Wrong answer");
            attempt++;
            while(attempt != 3)
            {
                printf("Your answer: ")
                scanf("%c",&ans);
                ans = toupper(ans);
            }
            printf("The correct answer is %c\n :", q.answer);
        }
        else{
            printf("Correct answer!");
        }
        
    }while (1);
}

int main()
{
    struct ask_questioins quest[3] = {
          {
              "What is the capital of France?",
              {"London", "Paris", "Berlin", "Madrid"},
              'B'
          },
          {
              "Which planet is known as the Red Planet?",
              {"Venus", "Mars", "Jupiter", "Saturn"},
              'B'
          },
          {
              "What is 2 + 2?",
              {"3", "4", "5", "6"},
              'B'
          }
      };
    
    int score = 0;
    printf("Welcome to the Quiz Game!\n");
    printf("You'll get 3 questions with three attempt each.\n");
    
    for (i=0; i<3; i++)
    {
        score = question(ques[i]);
    }
    
    return score;
    
}
