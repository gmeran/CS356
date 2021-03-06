#include<iostream>
#include<map>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
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

 graph->edge = (struct Edge*)malloc( graph ->E* sizeof(struct Edge));
 return graph;
}
void printArr(int dist[],int n)
{ 
 printf("Router   Distance from Router0\n");
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
  int V = 4;
  int E = 5;
  
  struct Graph* graph = createGraph(V,E);
  struct sockaddr_in server_addr;
  socklen_t size;

at map<string,int> routingTable;

  routingTable["Router0"]=1;
  routingTable["Router1"]=0;
  routingTable["Router2"]=1;
  routingTable["Router3"]=-1;

  //cout << routingTable["Router1"] << '\n';

// router0 to router1
   graph->edge[0].src = 0;
   graph->edge[0].dest = 1;
   graph->edge[0].weight = 1;

// router0 to router2
   graph->edge[1].src = 0;
   graph->edge[1].dest = 2;
   graph->edge[1].weight = 3;

// router0 to router3
   graph->edge[2].src = 0;
   graph->edge[2].dest = 3;
   graph->edge[2].weight = 7;

   BellmanFord(graph,0);
// Creating Socket

  client = socket(AF_INET,SOCK_STREAM,0);
 
  if(client < 0)
   { cout << "Couldn't Create Socket" << endl;
     exit(1);
   }

  cout << "Socket Created.." << endl;
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(portnum);

// Binding Socket / bind()
  
  if((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
   { cout << "Wasn't able to bind connection, socket already created..." <<endl;     return -1;
   }

   size = sizeof(server_addr);
   cout << "Looking for client..." <<endl;

// Listening Call / listen()
   
   listen(client,2);

// Accepting Clients
  
   int clientcnt = 2;
   server = accept(client,(struct sockaddr*)&server_addr,&size);

// checking if valid
   
   if(server < 0)
     cout << "Couldn't accept..." << endl;

   while(server > 0)
    { strcpy( buffer, " => Server Connected ...\n");
      send(server,buffer,bufsize,0);
      cout << "Connected with the client #" << clientcnt << endl;
      cout << "Enter to # to end connection\n" << endl;

      cout << "Client: ";

      do
       { recv(server,buffer,bufsize,0);
         cout << buffer << " ";
	 if(*buffer == '#')
	  { *buffer = '*';
	    isExit = true;
	  }	  
       } while(*buffer != '*');

       do
        { cout << "\nServer: ";
	  do
	   { cin >> buffer;
	     send(server,buffer,bufsize,0);
	     if(*buffer == '#')
	      { send(server,buffer,bufsize,0);
	        *buffer = '*';
		isExit = true;
	      }
	   }while(*buffer != '#');

	   cout << "Client: ";

	   do
	    { recv(server,buffer,bufsize,0);
	      cout << buffer << " ";
	      if(*buffer == '#')
	       { *buffer == '*';
	         isExit = true;
	       }
	      if(buffer [0] == 'R')
	       { 
		cout << "stuff" << endl;
	       } 
	    } while(*buffer != '*');
	  } while(!isExit);

// Close Call
        
          cout << "\n\n=> Connection with IP completed " << inet_ntoa(server_addr.sin_addr);
	  close(server);
	  cout << "\n Transmission Completed"<<endl;
	  isExit = false;
	  exit(1);
	}

	close(client);
	return 0;
}

