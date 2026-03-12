#include <iostream>

int main()
{

    int number = 3;

    if (number > 0)
    {
        std::cout << "Number is positive" << std::endl;
    }
    else if (number == 0)
    {
        std::cout << "Number is zero" << std::endl;
    }
    else
    {
        std::cout << "Number is negative" << std::endl;
    }

    // Day of the week
    // 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
    int day = 6; // try values from 1 to 7

    switch (day)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        std::cout << "Weekday" << std::endl;
        break;

    case 6:
    case 7:
        std::cout << "Weekend" << std::endl;
        break;

    default:
        std::cout << "Invalid day" << std::endl;
        break;
    }

    return 0;
}

// g++ -o selectionControlFlows selectionControlFlows.cpp && ./selectionControlFlows