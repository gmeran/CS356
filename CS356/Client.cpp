#include<iostream>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<stdio.h>
#include<limits.h>
using namespace std;

struct Edge
{
  int src, dest, weight;
};

struct Graph
{ 
  int V,E;
  
  struct Edge* edge;
};

struct Graph* createGraph(int V, int E)
{
  struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
  graph->V = V;
  graph->E = E;

  graph->edge = (struct Edge*)malloc(graph ->E* sizeof(struct Edge));
  return graph;
}

void printArr( int dist[], int n)
{
 printf("Router   Distance from Router1\n");
 for(int i = 0; i < n; ++i)
    printf("%d \t\t %d\n", i, dist[i]);
}

void BellmanFord(struct Graph* graph, int src)
{
 int V = graph->V;
 int E = graph->E;
 int dist[V];

 for(int i = 0; i < V; i++)
    dist[i] = INT_MAX;
 dist[src] = 0;

 for(int i = 1; i <= V-1; i++)
 {
   for(int j = 0; j < E; j++)
   { 
     int u = graph->edge[j].src;
     int v = graph->edge[j].dest;
     int weight = graph->edge[j].weight;
     if(dist[u] != INT_MAX && dist[u] + weight < dist[v])
       dist[v] = dist[u] + weight;
    }
 }
 
 for(int i = 0; i < E; i++)
  { int u = graph->edge[i].src;
    int v = graph->edge[i].dest;
    int weight = graph->edge[i].weight;
    if(dist[u] != INT_MAX && dist[u] + weight < dist[v])
      printf("Graph contains negative weight cylce");
  }

  printArr(dist,V);
  return;
}
int main()

{
  int client, server;
  int portnum = 1400;
  bool isExit = false;
  int bufsize = 1024;
  char buffer[bufsize];
  char *ip = "127.0.0.1";
  int V = 4;
  int E = 5;
 
  struct Graph* graph = createGraph(V,E);
  struct sockaddr_in server_addr;
  client =socket(AF_INET,SOCK_STREAM,0);

  
    

// router1 to router0
   graph->edge[0].src = 1;
   graph->edge[0].dest = 0;
   graph->edge[0].weight = 1;

// router1 to router2
   graph->edge[1].src = 1;
   graph->edge[1].dest = 2;
   graph->edge[1].weight = 1;

// router1 to router3
   graph->edge[2].src = 1;
   graph->edge[2].dest = 3;
   graph ->edge[2].weight = 3;
    
   BellmanFord(graph,1); 

// Creating Server Connection / Socket() 
 if(client < 0)
 { 
   cout << "\n Couldn't create socket.." << endl;
   exit(1);
 }
 
 cout << "\n=> Socket client has been created.." << endl;

 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(portnum);

 if(connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr))==0)
    cout << " => Connection to the server port number: " << portnum << endl;

 cout << " Waiting for Server confirmation..." << endl;
 recv(client,buffer,bufsize,0);
 cout << " Connection confirmed.." << endl;

 cout << "Enter # to end connection " << endl;
 
 do
  { cout << "Client: ";
   do
    { 
     cin >> buffer;
     send(client,buffer,bufsize,0);
     if(*buffer == '#')
      { 
       send(client,buffer,bufsize,0);
       *buffer = '*';
       isExit = true;
      }
     }while(*buffer != 42);
     
     cout << "Server :";

     do
      { recv(client,buffer,bufsize,0);
        cout << buffer << " ";
	if(*buffer == '#')
	 { *buffer = '*';
	   isExit = true;
	 }
      }while(*buffer != 42);

      cout << endl;
   }while(!isExit);

   
   cout << "Connection ended.." << endl;

   close(client);
   return 0;
}
     
  
     


