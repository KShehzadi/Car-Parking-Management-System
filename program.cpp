#include <iostream>
#include <ctime>
#include <vector>
#include <math.h> 
#include<fstream>
#include <sstream>
#include<time.h>
#include<algorithm>
using namespace std;

//path to csv file
string path="D:\\Car Parking Management System\\log.csv";


class csvManager
{
public:
    int maxRows;
    int maxCols;
    vector<vector<string>> fileContent;

    csvManager(int cols, int rows)
    {
        maxRows = rows;
        maxCols = cols;
    }
    void printCSVContent()
    {
        for (int i = 0; i < fileContent.size(); i++)
        {
            for (int j = 0; j < fileContent[i].size(); j++)
            {
                cout << fileContent[i][j] << "||\t";
            }
            cout<< endl;
        }
    }
   
    bool read(string filePath)
    {
        
        ifstream  iFile(filePath);
        bool read = iFile.good();
        string line;
        for (int i = 0; getline(iFile, line); i++)
        {
            if (maxRows < i + 1) maxRows = i + 1;
            stringstream  lineStream(line);
            string        cell;
            vector<string> cellsLine;
            for (int j = 0; getline(lineStream, cell, ','); j++)
            {
                if (maxCols < j + 1) maxCols = j + 1;
                cellsLine.push_back(rectifyString(cell));
            }
            fileContent.push_back(cellsLine);
        }
        iFile.close();
        refreshContent();
        return read;
    }
    string rectifyString(string str)
    {
        string result = "";
        for (int i = 0; i < str.length(); i++)
        {
            if (validateCharacter(str[i])) result += str[i];
        }
        return result;
    }

    bool validateCharacter(char c)
    {
        if (c > -1 && c < 255) return true;
        return false;
    }
    void refreshContent()
    {
        
        for (int i = 0; i < fileContent.size(); i++)
        {
            while (fileContent[i].size() < maxCols)
            {
                fileContent[i].push_back("");
            }
        }
    }



};


class Car{
    public:
    int carnumber;
    int cartype;
    time_t starttime = time(NULL);
    time_t exittime;
    Car()
    {

    }
    Car(int cnumber, int ctype)
    {
        carnumber = cnumber;
        cartype= ctype;
        //get the starting value of clock
        clock_t start = clock();
        tm* my_time;
        //get current time in format of time_t
        starttime = time(NULL);
        exittime = starttime;
    }
    int getHours()
    {
        return ceil(((exittime)/3600)-((starttime)/3600));
    }

};
class CarParking
{
    private:
    static const int numberofcartypes = 3;
    enum cartypes{Small_Car = 0 ,Car_2000CC = 1, Bus = 2};
    string carnames[numberofcartypes]={"Small Car","Car 2000 CC", "Bus or Truck"};
    int priceinthreehours[numberofcartypes]={20,30,70};
    int priceafterthreehours[numberofcartypes]={50,100,150};
    vector<Car> ParkedCars;
    public:
    
    CarParking()
    {
        
    }
    CarEntry()
    {
        cout<<"------------------Process Vehicle Entry Started------------------------"<<endl;
        int cnumber, ctype;
        cout<< "Enter Vehicle Number"<< endl;
        cin>> cnumber;
        for(int i=0; i< numberofcartypes; i++)
        {
            cartypes carname = static_cast<cartypes>(i);
            cout<<i+1<<"-"<<carnames[carname]<< endl;
        }
        cout<< "Enter Vehicle Type (1 for small car)"<<endl;
        cin>> ctype;
        if(ctype >= numberofcartypes && ctype < 0)
        {
            cout<< "Invalid Type"<< endl;
        }
        else
        {
            
            Car c(cnumber, ctype-1);
            ParkedCars.push_back(c);
            
        }
        
        cout<<"-------------------------Successfully Entered------------------------"<< endl;

    }
    CarExit()
    {
        cout<<"----------------------Process Vehicle Exit Started----------------------"<< endl;
        cout<<"Enter Vehicle Number"<<endl;
        int cnumber;
        cin>> cnumber;
        int carindex;
        for(int i = 0; i != ParkedCars.size(); i++) 
        {
            if(ParkedCars[i].carnumber==cnumber)
            {
                carindex=i;
                break;
            }
        }
        if(!ParkedCars[carindex].exittime)
        {
            cout<< "Already Exited"<<endl;
        }
        else
        {
            //get the starting value of clock
            clock_t start = clock();
            tm* my_time;
            ParkedCars[carindex].exittime=time(NULL);
            int hours=ParkedCars[carindex].getHours();
            if(hours == 0)
            {
                hours = 1;
            }
            PrintBill(hours, carindex);
        }
        
        cout<<"------------------------------------------------------------------------"<< endl;


    }
    PrintBill(int hours, int carindex)
    {
        double bill;
        if(hours <= 3)
        {
            bill = hours* priceinthreehours[(ParkedCars[carindex].cartype)];
        }
        else
        {
             bill = hours* priceafterthreehours[(ParkedCars[carindex].cartype)];
        }
        
        cout<< "Your Parking Bill is: "<<bill<< "Rs-/"<< endl;
        
    }
    ReturnBill(int hours, int carindex)
    {
        if(hours == 0)
        {
            hours = 1;
        }
        double bill;
        if(hours <= 3)
        {
            bill = hours* priceinthreehours[(ParkedCars[carindex].cartype)];
        }
        else
        {
             bill = hours* priceafterthreehours[(ParkedCars[carindex].cartype)];
        }
        return bill;

    }
    CheckParkingRates()
    {
         cout<<"---------------------------Car Parking Rates-----------------------------"<< endl;
        
        cout<< "Car Type"<<"\t"<<"1st Three Hours Price"<< "\t"<< "After Three Hours Price"<< endl;
        for(int i=0; i< numberofcartypes; i++)
        {
            cartypes carname = static_cast<cartypes>(i);
            cout<< carnames[carname]<<"\t"<<priceinthreehours[carname]<<"Rs/h\t\t\t"<<priceafterthreehours[carname]<<"Rs/h"<< endl;
        }
        cout<<"--------------------------------------------------------------------------"<< endl;

    }
    ExportToCSV()
    {
        
        cout<<"----------------------Process Exporting to CSV Started-------------------"<< endl;
       
        ofstream myfile;
        
        myfile.open (path);
        
        if(myfile.is_open())
        {
            
            myfile<<"Vehicle Number,Car Name, Start Time, Exit Time, Remarks,\n";
            for(int i=0; i < ParkedCars.size(); i++)
            {
              
                if(ParkedCars[i].exittime== ParkedCars[i].starttime)
                {
                    
                    std::ostringstream streamObjcarnumber;
                    //Add double to stream
                    streamObjcarnumber << ParkedCars[i].carnumber;
                    // Get string from output string stream
                    std::string strcarnumber = streamObjcarnumber.str();
                    char * temp= ctime(&ParkedCars[i].starttime);
                    int size=30;
                    size = std::remove_if(temp, temp + size, [](char c) { return c == '\n'; }) - temp;
                    temp[size] = 0;
                    //std::remove(temp, temp+strlen(temp), '\n') = '\0';
                
                    myfile<<strcarnumber<<","<<carnames[ParkedCars[i].cartype]<<","<<temp<<",Pending,"<<"Car is Still Parked,\n";
                }
                else
                {
                   
                    std::ostringstream streamObjcarnumber;
                    //Add double to stream
                    streamObjcarnumber << ParkedCars[i].carnumber;
                    // Get string from output string stream
                    std::string strcarnumber = streamObjcarnumber.str();


                    // Create an output string stream
                    std::ostringstream streamObj;
                    //Add double to stream
                    streamObj << ReturnBill(ParkedCars[i].getHours(),ParkedCars[i].cartype);
                    // Get string from output string stream
                    std::string strObj = streamObj.str();
                    
                    char * temp= ctime(&ParkedCars[i].starttime);
                    //*std::remove(temp, temp+strlen(temp), '\n') = '\0';
                    int size=30;
                    size = std::remove_if(temp, temp + size, [](char c) { return c == '\n'; }) - temp;
                    temp[size] = 0;
                    int size2= 30;
                    char * temp2= ctime(&ParkedCars[i].exittime);
                    //*std::remove(temp2, temp2+strlen(temp2), '\n') = '\0';
                    size2 = std::remove_if(temp2, temp2 + size2, [](char c) { return c == '\n'; }) - temp2;
                    temp2[size2] = 0;
                     
                    myfile<<strcarnumber<<","<<carnames[ParkedCars[i].cartype]<<","<<temp<<","<<temp2<<" ,"<<"Bill: "+strObj<<"Rs-/,\n";
                    
                }
                
            }
            myfile.flush();
            cout<<"Successfully Exported"<< endl;
        }
        else
        {
            cout<<"Path is not valid"<< endl;
        }
    
        cout<<"--------------------------------------------------------------------------"<< endl;


    }
    LoadFromCSV()
    {
        cout<<"----------------------Process Loading From CSV Started-------------------"<< endl;
        csvManager *reader= new csvManager(ParkedCars.size(),numberofcartypes);
        reader->read(path);
        reader->printCSVContent();
        
        cout<<"--------------------------------------------------------------------------"<< endl;

    }
    

};
string menu[6]={"Vehicle Entry", "Vehicle Exit","Check Parking Rates","Export To CSV","Load From CSV","Exit"};
void perform()
{
    CarParking *carParking = new CarParking();
    int menunumber;
    while (true)
    {
        cout<< "Please select menu:"<< endl;
        for(int i=0; i < 6; i++)
        {
            cout<< i+1<<"-"<<menu[i]<< endl;
        }
        cin>>menunumber;
        if(menunumber==1)
        {
            carParking->CarEntry();
        }
        else if( menunumber == 2)
        {
            carParking->CarExit();   
        }
        else if(menunumber== 3)
        {
           
            carParking->CheckParkingRates();
            
        }
        else if(menunumber== 4)
        {
            carParking->ExportToCSV();
        }
        else if(menunumber==5)
        {
            carParking->LoadFromCSV();
        }
        else if(menunumber==6)
        {
            cout<< "Exiting System"<< endl;
            break;
        }
        else
        {
            cout<< "Invalid value"<< endl;
        }
        menunumber=0;
        
    }
    

}
int main()
{
    perform();
}
