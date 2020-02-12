#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>

#include "scheduling.h"

using namespace std;

pqueue_arrival read_workload(string filename)
{
  pqueue_arrival workload;
  ifstream in;
  in.open(filename);
  if (!in.is_open())
  {
    perror("Cannot open workload file");
    return workload;
  }
  for (string arrival; !getline(in, arrival, ' ').eof();)
  {
    string duration;
    getline(in, duration);
    Process newP = {.arrival = stoi(arrival), .first_run = -1, .duration = stoi(duration), .completion = -1};
    workload.push(newP);
  }
  return workload;
}

void show_workload(pqueue_arrival workload)
{
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty())
  {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes)
{
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty())
  {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload)
{
  list<Process> complete;
  queue<Process> arrived;
  int time = 0;
  int timeend = 0;
  bool idle = true;
  while (workload.empty() && arrived.empty() && idle)
  {
    if (time == workload.top().arrival)
    {
      arrived.push(workload.top());
      workload.pop();
    }
    if (idle && !arrived.empty())
    {
      Process newP = arrived.front();
      arrived.pop();
      idle = false;
      timeend = time + newP.duration;
      newP.first_run = time;
      newP.completion = timeend;
      complete.push_back(newP);
    }
    else if (time == timeend)
    {
      idle = true;
    }
    ++time;
  }
  return complete;
}

list<Process> sjf(pqueue_arrival workload)
{
  list<Process> complete;
  pqueue_duration arrived;
  int time = 0;
  int timeend = 0;
  bool idle = true;
  while (workload.empty() && arrived.empty() && idle)
  {
    if (time == workload.top().arrival)
    {
      arrived.push(workload.top());
      workload.pop();
    }
    if (idle && !arrived.empty())
    {
      Process newP = arrived.top();
      arrived.pop();
      idle = false;
      timeend = time + newP.duration;
      newP.first_run = time;
      newP.completion = timeend;
      complete.push_back(newP);
    }
    else if (time == timeend)
    {
      idle = true;
    }
    ++time;
  }
  return complete;
}

list<Process> stcf(pqueue_arrival workload)
{
  list<Process> complete;
  pqueue_duration arrived;
  Process ongoing;
  int time = 0;
  int currDur = 0;
  bool idle = true;
  while (workload.empty() && arrived.empty() && idle)
  {
    if (time == workload.top().arrival)
    {
      arrived.push(workload.top());
      workload.pop();
    }
    if (!arrived.empty())
    {
      if (idle || arrived.top().duration < currDur)
      {
        if (!idle)
        {
          arrived.push(ongoing);
        }
        ongoing = arrived.top();
        arrived.pop();
        if (ongoing.first_run == -1)
        {
          ongoing.first_run = time;
          ongoing.completion = 0 - ongoing.duration;
        }
        idle = false;
      }
    }
    else if (ongoing.completion == 0)
    {
      ongoing.completion = time;
      complete.push_back(ongoing);
      idle = true;
    }
    if (!idle){
      ++ongoing.completion;
    }
    ++time;
  }
  return complete;
}

list<Process> rr(pqueue_arrival workload)
{
  list<Process> complete;
  queue<Process> arrived;
  int time = 0;
  Process ongoing;
  bool idle = true;
  while (workload.empty() && arrived.empty() && idle)
  {
    if (time == workload.top().arrival)
    {
      arrived.push(workload.top());
      workload.pop();
    }
    if (idle  && !arrived.empty())
    {
      ongoing = arrived.front();
      if (ongoing.first_run == -1)
      {
        ongoing.first_run = time;
        ongoing.completion = 0-ongoing.duration;
      }
    }
    else if (ongoing.completion == 0)
    {
      ongoing.completion = time;
      complete.push_back(ongoing);
      idle = true;
    }
    if (!idle) {
      ++ongoing.completion;
    }
    ++time;
  }
  return complete;
}

float avg_turnaround(list<Process> processes)
{
  float t = 0.0;
  int counter = 0;
  list<Process> output = processes;
  while (!output.empty())
  {
    Process each = processes.front();
    processes.pop_front();
    t += each.completion - each.arrival;
    ++counter;
  }
  return t/counter;
}

float avg_response(list<Process> processes)
{
  float t = 0.0;
  int counter = 0;
  list<Process> output = processes;
  while (!output.empty())
  {
    Process each = processes.front();
    processes.pop_front();
    t += each.completion - each.first_run;
    ++counter;
  }
  return t/counter;
}

void show_metrics(list<Process> processes)
{
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_turnaround(processes) << endl;
  cout << "Average Response Time:   " << avg_response(processes) << endl;
}
