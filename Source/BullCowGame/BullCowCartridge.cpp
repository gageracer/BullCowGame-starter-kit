// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    Words = GetValidWords(Words);
    SetupGame();
    
    PrintLine(TEXT("The number of possible words is %i."), Words.Num());
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    //if game is over Setup the Game
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }

    ProcessGuess(Input);
}

void UBullCowCartridge::SetupGame()
{   
    //Sets the variables
    HiddenWord = Words[FMath::RandRange(0,Words.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    //Welcome the player
    PrintLine(TEXT("Welcome to Bull Cows Game!\nGuess the %i letter word.\nYou have %i lives.\nPress Enter to continue."), HiddenWord.Len(), Lives);

}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Hidden word was:%s"),*HiddenWord);
    PrintLine(TEXT("\nGame Over.\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess){

    //Check if the guess is correct
    if (HiddenWord == Guess)
    {
        PrintLine(TEXT("You guessed right!"));
        EndGame();
        return;
    }

    //Wrong guess prompts
    PrintLine(TEXT("You guessed wrong!"));

    //Checks if guess length is correct
    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The hidden word is %i characters, try again."), HiddenWord.Len());
        return;
    }

    //Check if the guess is isogram
    if (!IsIsogram(Guess.ToLower()))
    {
        PrintLine(TEXT("That is not an isogram! Try again."));
        return;
    }

    //Check is any lives left
    if (--Lives == 0)
    {
        EndGame();
        return;
    }
    //FString Hint = GetHint(Guess);
    //Tells How much lives player has left and gives a hint
    PrintLine(TEXT("You have %i lives left!"), Lives);
    PrintLine(TEXT("You are so close:%s"), *GetHint(Guess));
}

bool UBullCowCartridge::IsIsogram(const FString& Guess) const{
    
    
    for (int32 Index = 0, Compare = 1; Index < Guess.Len() - 1; Compare++)
    {
        //PrintLine(TEXT("index is:%i,compare is:%i"), Index, Compare);
        if(Guess[Index] == Guess[Compare])
            return  false;
        //If Compare is at the end of the word, increment the Index and reset the Compare
        if(Compare == Guess.Len() - 1)
            {
                ++Index;
                Compare = Index;
            }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Lists) const
{
    TArray<FString> IsoWords;

    for (FString Word : Lists)
    {
        IsIsogram(Word) && IsoWords.Emplace(Word);
    }
    
    return IsoWords;
}


FString UBullCowCartridge::GetHint(const FString& MyGuess){

    FString Temp = MyGuess;
    for (int32 Index = 0; Index < MyGuess.Len(); Index++)
    {
        if(MyGuess[Index] != HiddenWord[Index])
            Temp[Index] = TEXT('_');
    }
    return Temp;
}