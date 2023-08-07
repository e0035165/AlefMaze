#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <functional>
#include <fstream>
#include <vector>
#include <list>

using namespace std;
string rtrim(const string &str);
string ltrim(const string &str);
vector<string> split(const string &str);
bool isPath(int x, int y, char**map);
bool isExit(int x, int y, char** map);
bool isTunnel(int x, int y, int** tunnels);
bool isMine(int x, int y, char**map);
bool isWall(int x, int y, char**map);
void handler(int xbound, int ybound, char**map, int**tunnels, pair<int,int>alefPosition);
int canAddEdge(int x, int y, int xbound, int ybound, char**map, int**tunnel);
void DFS(int x, char**map,  list<int>&lister, int** mapper);
float DFS(int x, char**map);
bool visited[400] = {false};
vector<vector<int>>graph(400);
float** vect;
list<float>rangeofpossibility;
float test = 0.00f;
int XB,YB;
vector<list<int>>combinatorics;


bool isWall(int x, int y, char**map)
{
    return map[x][y] == '#' ? true : false;
}
bool isMine(int x, int y, char**map)
{
    return map[x][y] == '*' ? true : false;
}
bool isTunnel(int x, int y, int** tunnels)
{
    return tunnels[x][y] == -1 ? false : true;
}

bool isExit(int x, int y, char** map)
{
    return map[x][y] == '%' ? true : false;
}

bool isPath(int x, int y, char**map)
{
    //cout << "In path" << endl;
    return map[x][y] == 'O' ? true : false;
}

int canAddEdge(int x, int y, int xbound, int ybound, char**map, int**tunnel)
{
    if(x < 0 || x >= xbound)
    {
        return -1;
    }
    
    if(y < 0 || y >= ybound)
    {
        return -1;
    }
    
    if(isWall(x,y,map))
    {
        return -1;
    }
    
    if(isMine(x,y,map) || isExit(x,y,map) || map[x][y] == 'A')
    {
        //cout << "Inside mine or exit" << (x*ybound+y) << endl;
        return (x*ybound)+y;
    } else
    {
        if(isTunnel(x, y, tunnel))
        {
            //cout << "Inside tunnel" << tunnel[x][y] << endl;
            return (tunnel[x][y]);
        } else {
            //cout << "Path" << (x*ybound)+y << endl;
            return (x*ybound)+y;
        }
    }
}

void DFS(int x, char**map, list<int>&lister, int** mapper)
{
    if(isExit(x/YB, x%YB, map))
    {
        list<int>newone = lister;
        combinatorics.push_back(newone);
        for(auto x=newone.begin();x!=newone.end();++x)
        {
            cout << *x << " ";
        }
        cout << endl;
        return;
    }
    mapper[x/YB][x%YB] = 1;
    visited[x] = true;
    int divider = 0;
    for(auto y=graph[x].begin();y!=graph[x].end();++y)
    {
        if(!visited[*y])
        {
            divider++;
        }
    }
    for(auto y=graph[x].begin();y!=graph[x].end();++y)
    {
        if(!visited[*y])
        {
            lister.push_back(*y);
            vect[(*y)/YB][(*y)%YB] += vect[x/YB][x%YB] * (float)(1.00f / (float) divider);
            DFS(*y,map,lister,mapper);
            lister.pop_back();
        } else {
            //vect[(*y)/YB][(*y)%YB] += (vect[x/YB][x%YB] * (float)(1.00f / (float) divider));
        }
        
    }
    //visited[x] = false;
    return;
}


void handler(int xbound, int ybound, char**map, int**tunnels, pair<int,int>alefPosition)
{
    XB = xbound;
    YB = ybound;
    vect = new float*[XB];

    int x = 0;
    while(x < xbound*ybound)
    {
        visited[x++] = false;
    }
    
    int** mapper = new int*[xbound];
    
    for(int i=0;i<xbound;++i)
    {
        mapper[i] = new int[ybound];
        vect[i] = new float[ybound];
        for(int j=0;j<ybound;++j)
        {
            vect[i][j] = 0.00f;
            mapper[i][j] = 0;
        }
    }
    
    
    for(int i=0;i<xbound;++i)
    {
        for(int j=0;j<ybound;++j)
        {
            
            if(isPath(i, j, map) || map[i][j] == 'A')
            {
                //cout << i << " " << j << endl;
                if(canAddEdge(i-1, j, xbound, ybound, map, tunnels) != -1)
                {
                    //cout << ans;
                    graph[i*ybound+j].push_back(canAddEdge(i-1, j, xbound, ybound, map, tunnels));
                }
                if(canAddEdge(i+1, j, xbound, ybound, map, tunnels) != -1)
                {
                    //cout << ans;
                    graph[i*ybound+j].push_back(canAddEdge(i+1, j, xbound, ybound, map, tunnels));
                }
                if(canAddEdge(i, j-1, xbound, ybound, map, tunnels) != -1)
                {
                    //cout << ans;
                    graph[i*ybound+j].push_back(canAddEdge(i, j-1, xbound, ybound, map, tunnels));
                }
                if(canAddEdge(i, j+1, xbound, ybound, map, tunnels) != -1)
                {
                    //cout << ans;
                    graph[i*ybound+j].push_back(canAddEdge(i, j+1, xbound, ybound, map, tunnels));
                }
//                cout << "For coordinate x: " << i << " y: " << j << " || ";
//                for(auto x=graph[i*ybound+j].begin();x!=graph[i*ybound+j].end();++x)
//                {
//                    cout << *x << " ";
//                }
//                cout << endl;
            }
        }
    }
    
    int alefPos = alefPosition.first*ybound + alefPosition.second;
    list<int>start;
    start.push_back(alefPos);
    vect[alefPosition.first][alefPosition.second] = 1.00f;
    DFS(alefPos,map,start,mapper);
    
    //cout << "Size of combinatorics: " << combinatorics.size() << endl;
    //list<int>pair = combinatorics[2];
    /*for(auto x=pair.begin();x!=pair.end();++x)
    {
        cout << *x << " ";
    }
    cout << endl;*/
    /*for(int i=0;i<xbound;++i)
    {
        for(int j=0;j<ybound;++j)
        {
            printf("%7.7f ", vect[i][j]);
        }
        cout << endl;
    }*/
    return;
    
    
}





int main(int argc, const char * argv[]) {
    fstream finout("Sonnet.txt", ios::in | ios::out);
    string first_multiple_input_temp;
    if(finout.is_open())
    {
        cout << "File detected\n";
        getline(finout, first_multiple_input_temp);
        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        int n = stoi(first_multiple_input[0]);

        int m = stoi(first_multiple_input[1]);

        int k = stoi(first_multiple_input[2]);
        pair<int,int>alefPosition;
        char** map = new char*[n];
        int** tunneller = new int*[n];
        for (int n_itr = 0; n_itr < n; n_itr++) {
            string row;
            getline(finout, row);
            map[n_itr] = new char[m];
            tunneller[n_itr] = new int[m];
            for(int m_itr=0;m_itr<m;++m_itr)
            {
                tunneller[n_itr][m_itr] = -1;
                //tunneller[n_itr][m_itr].second = -1;
                map[n_itr][m_itr] = row[m_itr];
                if(map[n_itr][m_itr] == 'A')
                {
                    alefPosition.first = n_itr;
                    alefPosition.second = m_itr;
                }
                
            }
            
        }
        
        for (int k_itr = 0; k_itr < k; k_itr++) {
            string second_multiple_input_temp;
            getline(finout, second_multiple_input_temp);

            vector<string> second_multiple_input = split(rtrim(second_multiple_input_temp));
            //cout << second_multiple_input_temp << endl;
            int i1 = stoi(second_multiple_input[0]);

            int j1 = stoi(second_multiple_input[1]);

            int i2 = stoi(second_multiple_input[2]);

            int j2 = stoi(second_multiple_input[3]);
            tunneller[i1-1][j1-1] = (i2-1)*m + (j2-1);
            tunneller[i2-1][j2-1] = m*(i1-1) + (j1-1);
        }
        handler(n, m, map, tunneller, alefPosition);
    } else {
        cout << "No file detected\n";
    }
    
    
    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
    return s;

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
    return s;

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
