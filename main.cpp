#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Task{
    string name;
    int difficulty;
    int duration;
    int deadline;
    vector<int>dependencies;
    int priorityScore;
};

class Node{
    public:
    int data;
    Node* next;

    Node(int val){
        data = val;
        next = NULL;
    }
};

class Queue{
    Node* front;
    Node* back;

    public:
    Queue(){
        front = NULL;
        back = NULL;
    }

    void push(int x){
        Node* n = new Node(x);
        if(front == NULL){
            back = n;
            front = n;
            return;
        }
        back->next = n;
        back = n;
    }

    int pop(){
        if(front == NULL){
            return -1;
        }
        Node* toDelete = front;
        int value = front->data;
        front = front->next;
        delete toDelete;
        return value;
    }

    bool empty() {
        return front == NULL;
    }
};

class maxHeap{
    public:
    int size;
    int capacity;
    int* taskIndex;
    double* priority;

    maxHeap(int cap = 100){
        capacity = cap;
        size = 0;
        taskIndex = new int[capacity];
        priority = new double[capacity];
    }

    int parent(int i){
        return (i-1)/2;
    }

    int leftChild(int i){
        return 2*i+1;
    }

    int rightChild(int i){
        return 2*i+2;
    }

    void insert(int idx, double pri){
        if(size == capacity){
            cout << "heap full!" << endl;
            return;
        }
        taskIndex[size] = idx;
        priority[size] = pri;
        int i = size;
        size++;

        while(i != 0 && priority[parent(i)] < priority[i]){
            swap(priority[i], priority[parent(i)]);
            swap(taskIndex[i], taskIndex[parent(i)]);
            i = parent(i);
        }
    }

    void maxHeapify(int i){
        int largest = i;
        int l = leftChild(i);
        int r = rightChild(i);

        if(l<size && priority[l] > priority[largest]){
            largest = l;
        }

        if(r<size && priority[r] > priority[largest]){
            largest = r;
        }

        if(largest != i){
            swap(priority[i], priority[largest]);
            swap(taskIndex[i], taskIndex[largest]);
            maxHeapify(largest);
        }
    }

    void removeMax(){
        if(size <= 0){
            cout << "heap empty!" << endl;
            return;
        }

        //cout << "removed task index: " << taskIndex[0] << " with priority " << priority[0] << endl;
        taskIndex[0] = taskIndex[size-1];
        priority[0] = priority[size-1];
        size--;
        maxHeapify(0);
    }

    bool empty(){
        return size == 0;
    }

    ~maxHeap(){
        delete[] taskIndex;
        delete[] priority;
    }
};

void inputTasks(vector<Task>&tasks){
    int n;
    cout << "enter number of tasks: ";
    cin >> n;
    cout << endl;

    tasks.resize(n);

    for(int i=0;i<n;i++){
        cout << "-----task " << i << "-----" << endl;

        cout << "enter task name: ";
        cin >> tasks[i].name;

        cout << "enter deadline(in days): ";
        cin >> tasks[i].deadline;

        cout << "enter difficulty(1 to 10): ";
        cin >> tasks[i].difficulty;

        cout << "enter hours required: ";
        cin >> tasks[i].duration;

        int d;
        cout << "number of dependencies for this task: ";
        cin >> d;

        cout << "enter dependency indexes ( 0 to " << n-1 << "): ";
        for(int j=0;j<d;j++){
            int x; 
            cin >> x;
            tasks[i].dependencies.push_back(x);
        }
        cout << endl;
    }
}

int computePriorityScore(Task t){
    return ( (1.0/t.deadline)+t.difficulty );
}

// void merge(vector<Task>&arr, int start, int mid, int end){
//     vector<Task>temp;
//     int i = start;
//     int j = mid + 1;

//     while(i<=mid && j<=end){
//         if(arr[i].priorityScore >= arr[j].priorityScore){
//             temp.push_back(arr[i]);
//             i++;
//         }else{
//             temp.push_back(arr[j]);
//             j++;
//         }
//     }

//     while(i<=mid){
//         temp.push_back(arr[i]);
//         i++;
//     }

//     while(j<=end){
//         temp.push_back(arr[j]);
//         j++;
//     }

//     for(int index=0; index < temp.size(); index++){
//         arr[index+start] = temp[index];
//     }
// }

// void mergeSort(vector<Task>&arr, int start, int end){
//     if(start < end){
//         int mid = start + (end-start)/2;
//         mergeSort(arr, start, mid);
//         mergeSort(arr, mid+1, end);
//         merge(arr, start, mid, end);
//     }
// }

void buildGraph(const vector<Task>& tasks,vector<vector<int>>& graph,vector<int>& indegree)
{
    int n = tasks.size();

    graph.assign(n, vector<int>()); 
    indegree.assign(n, 0);

    for(int i = 0; i < n; i++){
        for(int dep : tasks[i].dependencies){
            graph[dep].push_back(i);
            indegree[i]++;
        }
    }
}

vector<int> topologicalSort(const vector<Task>&tasks,vector<vector<int>>& graph,vector<int>& indegree){
    Queue q;
    int n = tasks.size();

    for(int i=0;i<n;i++){
        if(indegree[i] == 0){
            q.push(i);
        }
    }

    vector<int> topoOrder;

    while(!q.empty()){
        int node = q.pop();
        topoOrder.push_back(node);

        for(int neighbor : graph[node]){
            indegree[neighbor]--;
            if(indegree[neighbor] == 0){
                q.push(neighbor);
            }
        }
    }

    if(topoOrder.size() != n){
        cout << "error: cycle detected in tasks dependencies!" << endl;
    }

    return topoOrder;
}

vector<vector<int>> scheduleTaks(vector<Task>&tasks, vector<int>&topoOrder, int HOURS_PER_DAY = 8){
    int n = tasks.size();
    vector<bool> completed(n,false);
    vector<vector<int>> dailyPlan;
    int tasksRemaining = n;

    while(tasksRemaining > 0){
        int hoursLeft = HOURS_PER_DAY;
        vector<int> today;
        maxHeap heap(n);

        for(int idx: topoOrder){
            if(completed[idx]){
                continue;
            }
            bool canDo = true;
            for(int dep: tasks[idx].dependencies){
                if(!completed[dep]){
                    canDo = false;
                    break;
                }
            }
            if(canDo){
                heap.insert(idx, tasks[idx].priorityScore);
            }
        }

        while(!heap.empty() && hoursLeft > 0){
            int taskIdx = heap.taskIndex[0];
            int dur = tasks[taskIdx].duration;

            if(dur <= hoursLeft){
                today.push_back(taskIdx);
                hoursLeft -= dur;
                completed[taskIdx] = true;
                tasksRemaining--;
                heap.removeMax();
            }else{
                break;
            }
        }
        dailyPlan.push_back(today);
    }
    return dailyPlan;
}

void printOutput(const vector<Task>&tasks, const vector<vector<int>>&dailyPlan){
    cout << "-----FINAL DAILY SCHEDULE-----"<< endl;
    for(int day = 0;day < dailyPlan.size(); day++){
        cout << "Day " << day+1 << ": ";

        for(int idx: dailyPlan[day]){
            cout << tasks[idx].name << " ";
        }
        cout << endl;
    }
}

int main(){
    vector<Task> tasks;
    inputTasks(tasks);

    for(int i=0;i<tasks.size();i++){
        tasks[i].priorityScore = computePriorityScore(tasks[i]);
    }

    //mergeSort(tasks, 0, tasks.size()-1);

    vector<vector<int>> graph;
    vector<int> indegree;
    buildGraph(tasks,graph,indegree);

    vector<int> topoOrder = topologicalSort(tasks,graph,indegree);

    vector<vector<int>> dailyPlan = scheduleTaks(tasks, topoOrder);

    printOutput(tasks, dailyPlan);

    return 0;
}