#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
### File:     Abd-vocab.py
### Date:     2024-10-25
### Authors:  NN, University of Potsdam, 2024
### Homework: Week 2 - Vocabulator

import sys
import random

# Declare ANSI colors for feedback
RED = "\033[31m"    
GREEN = "\033[32m" 
RESET = "\033[0m"   

# Lists for German and English words
Ger = ["Apfel", "Obst", "Straße", "Katze", "Brücke", "Erfahrung", "Anschluss", "Erlebnis", "Gebiet", "knüpfen"]
Eng = ["Apple", "Fruit", "Street", "Cat", "Bridge", "Experience", "Connection", "Adventure", "Area", "To knot"]

def menu():
    while True:
        choice = input("Translate from English to German (e), German to English (d), or quit (q): ").strip().lower()
        if choice == 'd':
            return "English"
        elif choice == 'e':
            return "German"
        elif choice == 'q':
            print("Exiting the program.")
            sys.exit()
        else:
            print(f"{RED}Invalid input. Please choose 'e', 'd', or 'q'.{RESET}")

def translate(lang):
    # Generate a random index for a word in the list
    random_index = random.randint(0, len(Ger) - 1)
    
    if lang == "English":
        question_word = Ger[random_index]
        correct_answer = Eng[random_index]
    else:  
        question_word = Eng[random_index]
        correct_answer = Ger[random_index]

    # Ask the user for the translation
    print(f"Translate this word: {question_word}")
    user_answer = input("Your answer: ").strip()

    # Check the answer
    if user_answer.lower() == correct_answer.lower():
        print(f"{GREEN}Correct!{RESET}")
        return True
    else:
        print(f"{RED}Incorrect! The correct translation is: {correct_answer}{RESET}")
        return False

def main():
    print("Welcome to the Translator!")
    correct_count = 0
    num_questions = 5  

    for _ in range(num_questions):
        lang = menu()  
        if translate(lang):  
            correct_count += 1

    print(f"\nYou answered {correct_count} out of {num_questions} correctly.")
    print("Thank you!")

if __name__ == "__main__":
    main()
