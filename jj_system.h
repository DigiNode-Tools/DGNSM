static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

// memory
int parseLine(char *line)
{
    int i = strlen(line);
    const char *p = line;
    while (*p < '0' || *p > '9')
        p++;
    line[i - 3] = '\0';
    i = atoi(p);
    return i;
}

int getmemramuse()
{
    unsigned long total_memory;
    unsigned long used_memory = 0;
    FILE *file = fopen("/proc/meminfo", "r");
    int result = -1;
    char line[128];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "MemTotal:", 9) == 0)
        {
            sscanf(line + 9, "%lu", &total_memory);
        }
        else if (strncmp(line, "MemAvailable:", 13) == 0)
        {
            unsigned long available_memory;
            sscanf(line + 13, "%lu", &available_memory);
            result = total_memory - available_memory;
            result = result / 1024;
            break;
        }
    }
    fclose(file);
    return result;
}

int getmemramtotal()
{
    unsigned long total_memory;
    FILE *file = fopen("/proc/meminfo", "r");
    int result = -1;
    char line[128];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "MemTotal:", 9) == 0)
        {
            sscanf(line + 9, "%lu", &total_memory);
        }
        else if (strncmp(line, "MemAvailable:", 13) == 0)
        {
            unsigned long available_memory;
            sscanf(line + 13, "%lu", &available_memory);
            result = total_memory;
            result = result / 1024;
            break;
        }
    }
    fclose(file);
    return result;
}

int getmemrss()
{
    FILE *file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];
    while (fgets(line, 128, file) != NULL)
    {
        if (strncmp(line, "VmRSS:", 6) == 0)
        {
            result = parseLine(line);
            result = result / 1000;
            break;
        }
    }
    fclose(file);
    return result;
}

// memory
// cpu
void init()
{
    FILE *filex = fopen("/proc/stat", "r");
    if (fscanf(filex, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
               &lastTotalSys, &lastTotalIdle))
        ;
    fclose(filex);
}

double getcpu()
{
    double percent;
    FILE *filex;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    filex = fopen("/proc/stat", "r");
    if (fscanf(filex, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
               &totalSys, &totalIdle))
        ;
    fclose(filex);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle)
    {
        percent = -1.0;
    }
    else
    {
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
                (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;
    return percent;
}
// cpu
