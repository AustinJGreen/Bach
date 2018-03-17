//
// Created by austi on 12/9/2017.
//

#include "score.h"

score::score()
{

}

void score::swap(float arr[], int indicies[], int a, int b) const
{
    float temp1 = arr[a];
    int temp2 = indicies[a];
    arr[a] = arr[b];
    arr[b] = temp1;
    indicies[a] = indicies[b];
    indicies[b] = temp2;
}

void score::quickSort(float arr[], int indicies[], int low, int high) const
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, indicies, low, high);

        quickSort(arr, indicies, low, pi - 1);  // Before pi
        quickSort(arr, indicies, pi + 1, high); // After pi
    }
}

int score::partition(float arr[], int indicies[], int low, int high) const
{
    // pivot (Element to be placed at right position)
    float pivot = arr[high];

    int i = low - 1;  // Index of smaller element
    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] >= pivot)
        {
            i++;    // increment index of smaller element
            swap(arr, indicies, i, j);
        }
    }

    swap(arr, indicies, i + 1, high);
    return i + 1;
}

float score::getVector(int player) const
{
    float copy[TEAM_COUNT];
    int indicies[TEAM_COUNT];
    for (int j = 0; j < TEAM_COUNT; j++)
    {
        copy[j] = values[j];
        indicies[j] = j;
    }

    quickSort(copy, indicies, 0, TEAM_COUNT - 1);
    int j = indicies[player];
    if (j == 0)
    {
        return copy[0] - copy[1];
    }
    else
    {
        return copy[j] - copy[0];
    }
}

float score::operator[](int index)
{
    assert(index >= 0 && index < TEAM_COUNT);
    return values[index];
}

void score::set(int player, float value)
{
    assert(player >= 0 && player < TEAM_COUNT);
    values[player] = value;
}