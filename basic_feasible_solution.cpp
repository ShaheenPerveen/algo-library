/* A Program to find the Basic Feasible Solutions and find the Maximum value of the Objective Function
 * Author : Ankesh Anand
 * Roll No. : 11MA20052
 * Operations Research Laboratory
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<set>
using namespace std;

void gauss_elim(vector<vector<double> >& sq_matrix, vector<double>& matrix_B,int m, int n);
void swap_rows(vector<vector<double> >& sq_matrix,int a, int b,int n);
int back_substitution(vector<vector<double> >& sq_matrix, vector<double>& matrix_B, vector<double>& matrix_X, int n);
void print_matrix(vector< vector<double> >sq_matrix,int n);

int main()
{
    int m,n,nbasic_var;
    vector< vector<double> > matrix_A;
    vector<double> matrix_right;
    vector<double> matrix_X;
    //Input Operations
    cout << "Enter the number of variables" << endl;
    cin >> m;
    cout << "Enter the number of equations" << endl;
    cin >> n;
    cout << "Enter a Matrix of size" << n << "*" << m << endl;
    for(int i=0;i<n;i++)
    {
        vector<double> row;
        for(int j=0;j<m;j++)
        {
            double temp;
            cin >> temp;
            row.push_back(temp);
        }
        matrix_A.push_back(row);
    }
    cout << "Enter the matrix b" << endl;
    for(int i=0;i<n;i++)
    {
        double temp;
        cin >> temp;
        matrix_right.push_back(temp);
    }
    cout << "Enter the function to maximize(seperate coeffiecients of maximizing function by spaces)" << endl;
    vector<double> matrix_Z(m,0);
    for(int i=0;i<m-n;i++)
    {
        double temp;
        cin >> temp;
        matrix_Z[i] = temp;
    }
    nbasic_var = m - n;
    vector<int> generator;
    for(int i=0;i<m;i++)
    {
        generator.push_back(i+1);
    }
    // Generating All Possible Combinations of Non-Basic Variables
    set< vector<int> > combinations;
    do{
        vector<int> temp(generator.begin(), generator.begin()+nbasic_var);
        sort(temp.begin(),temp.end());
        combinations.insert(temp);
    }while(next_permutation(generator.begin(),generator.end() ));

    set< vector<int> >::iterator it;

    cout << endl;
    cout << "The Solutions Are:" << endl;
    int count = 1; 
    double max = 0;
    for(it = combinations.begin(); it != combinations.end(); it++)
    {
        cout << count << ". ";
        vector<int> temp(*it);
        vector< vector<double> > sq_matrix;
        vector<double> matrix_B(matrix_right);
        for(int i=0;i<n;i++)
        {
            vector<double> row;
            for(int j=0;j<m;j++)
            {
                vector<int>::iterator it = find(temp.begin(),temp.end(),j+1);
                if(it != temp.end())
                {
                    continue;
                }
                row.push_back(matrix_A[i][j]);
            }
            sq_matrix.push_back(row);
        }
        cout << "Non Basic Variables: "; 
        for(int i=0;i<m;i++)
        {
            vector<int>::iterator it = find(temp.begin(),temp.end(),i+1);
            if(it != temp.end())
            {
                cout << "x" << i+1 << "=0, ";
            }

        }
        gauss_elim(sq_matrix,matrix_B,m,n);
        if(back_substitution(sq_matrix,matrix_B,matrix_X,n)==-1)
        {
            count++;
            continue;
        }
        vector<int> basic_variables;

        for(int i=0;i<m;i++)
        {
            vector<int>::iterator it = find(temp.begin(),temp.end(),i+1);
            if(it == temp.end())
            {
                basic_variables.push_back(i+1);
            }
        }
        cout << endl;
        cout << "    Basic Variables: ";
        double z = 0; 
        for(int i=0;i<n;i++)
        {
            cout << "x" << basic_variables[i] << "=" << matrix_X[i] << ",";
            if(matrix_X[i] < 0)
            {
                cout << endl << "    (Not a Basic Feasible Solution.)" << endl;
                z = -1;
                break;
            }
            z = z + matrix_Z[basic_variables[i]-1]*matrix_X[i];
        }
        if(z >=0)
        {
            cout << endl << "    (It is a Basic Feasible Solution.) Z=" << z << endl;
        }
        if(z > max)
        {
            max = z;
        }
        count++;
        cout << endl;
    }
    cout << "The Final Solution is Z = " << max << endl;
}

void gauss_elim(vector<vector<double> >& sq_matrix, vector<double>& matrix_B,int m, int n)
{
    for(int k=0;k<n;k++)
    {
        //Find Pivot for Column k
        double max = sq_matrix[k][k];
        int i_max = k;
        for(int t=k;t<n;t++)
        {
            if(sq_matrix[t][k] > max)
            {
                max = sq_matrix[t][k];
                i_max = t;
            }
        }
        if(sq_matrix[i_max][k] == 0)
        {
            cout << "The Matrix is Singular"  << endl;
            //print_matrix(sq_matrix,n);
            return;
        }
        swap_rows(sq_matrix,k,i_max,n);
        double temp = matrix_B[k];
        matrix_B[k] = matrix_B[i_max];
        matrix_B[i_max] = temp;
        //Do for All Rows Below Pivot:
        for(int i=k+1;i<n;i++)
        {
            //Do for all elements in current row
            double temp = sq_matrix[i][k];
            for(int j=k;j<n;j++)
            {
                sq_matrix[i][j] = sq_matrix[i][j] - sq_matrix[k][j] * (double)(temp/(int)sq_matrix[k][k]);
            }
            matrix_B[i] = matrix_B[i] - matrix_B[k] * (temp/sq_matrix[k][k]);
            //Fill lower triangular matrix with zeros
            sq_matrix[i][k] = 0;
        }        
    }
}

/*Helper Function to Swap Rows*/
void swap_rows(vector<vector<double> >&sq_matrix,int a, int b,int n)
{
    for(int i=0;i<n;i++)
    {
        double temp = sq_matrix[a][i];
        sq_matrix[a][i] = sq_matrix[b][i];
        sq_matrix[b][i] = temp;
    }
}

/*Function for Back Substitution*/
int back_substitution(vector<vector<double> >& sq_matrix, vector<double>& matrix_B, vector<double>& matrix_X, int n)
{
    matrix_X.resize(n);
    for(int i=n-1;i>=0;i--)
    {
        for(int j=i+1;j<n;j++)
        {
            matrix_B[i] = matrix_B[i] - sq_matrix[i][j]*matrix_X[j];
        }
        if(sq_matrix[i][i] == 0)
        {
            //cout << "Dividing by Zero" << endl;
            //The Matrix is Singular
            return -1;
        }
        matrix_X[i] = matrix_B[i] / sq_matrix[i][i];
    }
}

void print_matrix(vector< vector<double> >sq_matrix,int n)
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cout << sq_matrix[i][j] << " ";
        }
        cout << endl;
    }
}
