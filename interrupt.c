int flag = 0;
// the int fn
    read core timer into t
    wait for core timer to be t + 1ms
    check if button is pushed (port == 0)
    {
        if first time pushed (flag == 0)
            set core timer to 0
            set flag to 1
        if (flag == 1)
            read core timer
            print to user how much time has passed
            set flag to 0
    }

    else
    {
        nothing
    }

clear the int flag