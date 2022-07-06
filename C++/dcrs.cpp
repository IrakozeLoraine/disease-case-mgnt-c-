#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<thread>
#include<set>

using namespace std;
using namespace std::chrono;

class DiseaseRecord {
    public:
        string location;
        string disease;
        int cases;
        
        DiseaseRecord(){
        	
		}
        DiseaseRecord(string loc){
        	location = loc,
        	disease = "",
        	cases = 0;
		}

        DiseaseRecord(string loc, string dis, int case_no) {
            location = loc;
            disease = dis;
            cases = case_no;
        }
        
        string to_string() {
        	return location + "," + disease + "," + std::to_string(cases);
        	
		}
};

bool is_file_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

bool is_string_an_integer(string str){
	
	int n = str.length();
 
    char char_array[n + 1];
 
    strcpy(char_array, str.c_str());
	
    for (int i = 0; i < strlen(char_array); i++) {
        if(!isdigit(char_array[i])){
            return false;
        }
    }
    return true;
}

void tokenize_string(string const &str, const char delim, vector<string> &out)
{
	// construct a stream from the string
	stringstream ss(str);
	string token;
	while (getline(ss, token, delim))
	{
        out.push_back(token);
	}
}

string find_record(string record){
    fstream file;
    file.open("records.csv", ios::in);
    string line;
    string result = "";
    //find location in file
    while(getline(file, line)){
        if(line.find(record) != string::npos){
            result = line;
        }
    }
    file.close();
    
    return result;
}

void save_record(DiseaseRecord record) {
    fstream file;
    file.open("records.csv", ios::in);
    
    if(!file){
        file.close();
        file.open("records.csv", ios::out);
        file<<"Locations,Disease,Cases"<<endl;
    } else {
        file.close();
        file.open("records.csv", ios::app);
    }
    
    file<<record.to_string()<<endl;

    file.close();
}

vector<DiseaseRecord> load_records_from_file(){
    vector<DiseaseRecord> records;
    ifstream file;
    file.open("records.csv");
    string line = "";

    if(!is_file_empty(file)){
        while(getline(file, line)){

            if(line.find("Location") != string::npos){
                continue;
            }

            stringstream ss(line);
            string token;
            vector<string> tokens;
            while(getline(ss, token, ',')){
                tokens.push_back(token);
            }
            
            DiseaseRecord record;
            
            record.location = tokens[0];
            record.disease = tokens[1];

            //check if tokens[2] is digit
            if(is_string_an_integer(tokens[2])){
                record.cases = stoi(tokens[2]);
            } else {
            	cout<<"cases must be in numbers"<<endl;
                record.cases = 0;
            }
            records.push_back(record);
        }
    }
    
    file.close();
    return records;
}

void list_locations(){
	vector<DiseaseRecord> records = load_records_from_file();

    if (records.empty()){
        cout<<"No records found"<<endl;
    } else {

        //sort vector alphabetically
        sort(records.begin(), records.end(), [](DiseaseRecord a, DiseaseRecord b){
            return a.location < b.location;
        });

        set<string> locations;

        for(auto record : records){
            locations.insert(record.location);
        }

        for (auto itr = locations.begin(); itr != locations.end(); itr++) {
            cout <<*itr<<endl;
        }
    }
}

void list_diseases(){
	vector<DiseaseRecord> records = load_records_from_file();

    if (records.empty()){
        cout<<"No records found"<<endl;
    } else {

        //sort vector alphabetically
        sort(records.begin(), records.end(), [](DiseaseRecord a, DiseaseRecord b){
            return a.disease < b.disease;
        });

        set<string> diseases;

        for(auto record : records){
        	if(record.disease!=""){
				diseases.insert(record.disease);	
			}
        }

        for (auto itr = diseases.begin(); itr != diseases.end(); itr++) {
            cout <<*itr<<endl;
        }
    }
}

void delete_location(string loc){
    string location = find_record(loc);
    
    if(location == ""){
        cout<<"Location not found"<<endl;
    } else {
        vector<DiseaseRecord> records = load_records_from_file();
        if (records.empty()){
            cout<<"No records found"<<endl;
        } else {
            for (auto i = 0; i < records.size(); i++)
            {
                if (records[i].location == loc)
                {
                    records.erase(records.begin() + i);
                }
            }
            
            remove("records.csv");
            for (auto i = 0; i < records.size(); i++)
            {
                save_record(records.at(i));
            }
        	cout<<"location "<<loc<<" is successfully deleted!"<<endl;
        }
    }
}

void add_disease(string loc, string disease, int cases) {
    string location = find_record(loc);
    
    if(location == ""){
        cout<<"Location not found"<<endl;
    } else {
        vector<DiseaseRecord> records = load_records_from_file();
        
        if (records.empty()){
            cout<<"No records found"<<endl;
        } else {
            DiseaseRecord newRecord = DiseaseRecord(loc, disease, cases);

            for (int i = 0; i < records.size(); i++) 
            {
                if (records[i].location == loc && records[i].disease == "")
                {
                    records.erase(records.begin() + i);
                }
            }

            records.push_back(newRecord);

            remove("records.csv");
            for (auto i = 0; i < records.size(); i++)
            {
                save_record(records.at(i));
            }
        }
    }
}

void add_location(string location){
    DiseaseRecord record = DiseaseRecord(location);
    save_record(record);
}

void find_disease_location(string dis){
    string disease = find_record(dis);
    if(disease == ""){
        cout<<"No location with this disease"<<endl;
    } else {
        vector<DiseaseRecord> records = load_records_from_file();
        if (records.empty()){
            cout<<"No records found"<<endl;
        } else {
            vector<string> locations;
            for (auto i = 0; i < records.size(); i++)
            {
                if (records[i].disease == dis)
                {
                    locations.push_back(records[i].location);
                }
            }

            if(locations.empty()){
                cout<<"No location with this disease"<<endl;
            } else {
                 cout<<"[";
                for (auto i = 0; i < locations.size(); i++)
                {
                    transform(locations.at(i).begin(), locations.at(i).end(), locations.at(i).begin(), ::tolower);
                    cout<<locations.at(i);
                    if(i != locations.size()-1){
                        cout<<",";
                    }
                }
                cout<<"]";
            }
        }
    }
}

void find_location_cases(string loc, string dis){
    string location = find_record(loc);
    
    if(location == ""){
        cout<<"Location not found"<<endl;
    } else {
        vector<DiseaseRecord> records = load_records_from_file();
        if (records.empty()){
            cout<<"No records found"<<endl;
        } else {
            int location_cases = 0;
            for (auto i = 0; i < records.size(); i++)
            {
                if (records[i].location == loc && records[i].disease == dis)
                {
                    location_cases = records[i].cases;
                    break;
                }
            }
            cout<<"Cases of "<<dis<<" at "<<loc<<" are: "<<location_cases<<endl;
        }
    }
}

void find_total_cases(string dis){
    string disease = find_record(dis);
    if(disease == ""){
        cout<<"No location with this disease"<<endl;
    } else {
        vector<DiseaseRecord> records = load_records_from_file();
        if (records.empty()){
            cout<<"No records found"<<endl;
        } else {
            int total_cases = 0;
            for (auto i = 0; i < records.size(); i++)
            {
                if (records[i].disease == dis)
                {
                    total_cases +=records[i].cases;
                }
            }
            cout<<"Total cases of '"<<dis<<"' = "<<total_cases<<endl;
        }
    }
}

void user_manual(){
    cout<<"=============================================================================="<<endl;
    cout<<"*\t\t\t\t    HELP MENU    \t\t\t\t*"<<endl;
   	cout<<"=============================================================================="<<endl;

    cout<<"add <Location> \t\t\t\t: Add a new location"<<endl;
    cout<<"delete <Location> \t\t\t: Delete an existing"<<endl;
    cout<<"record <Location> <disease> <cases> \t: Record a disease and its cases"<<endl;
    cout<<"list locations \t\t\t\t: List all existing locations"<<endl;
    cout<<"list diseases \t\t\t\t: List existing Diseases in locations"<<endl;
    cout<<"where <diseases>\t\t\t: Find where disease exists"<<endl;
    cout<<"cases <location> <disease>\t\t: Find cases of a disease in location"<<endl;
    cout<<"cases <disease> \t\t\t: Find total cases of a given disease"<<endl;
    cout<<"help\t\t\t\t\t: Prints user manual"<<endl;
    cout<<"Exit\t\t\t\t\t: Exit the program"<<endl;
}

void list_disease_cases(){
    vector<DiseaseRecord> records = load_records_from_file();

    if (records.empty()){
        cout<<"No records found"<<endl;
    } else {

        //sort vector alphabetically
        sort(records.begin(), records.end(), [](DiseaseRecord a, DiseaseRecord b){
            return a.disease < b.disease;
        });
        
        cout<<endl;

        for(auto record : records){
        	if(record.disease!=""){
				cout<<record.disease<<" => "<<record.location<<" => "<<record.cases<<endl;
			}
        }
    }
}

void summary(){
    auto start = high_resolution_clock::now();

    list_disease_cases();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
 
    cout <<endl<< "Processing time: "
         << duration.count() << " microseconds" << endl;
}

int main(){
    string option;
do
{
    cout<<endl<<"========================================================="<<endl<<endl;
    cout<<"*\t Welcome to Disease Cases Reporting System!\t*"<<endl<<endl;
    cout<<"* ***************************************************** *"<<endl<<endl;
    cout<<"*\t\t\t\t\t\t\t*"<<endl<<endl;
    cout<<"*It is developed by Loraine Mukezwa Irakoze as practical*"<<endl<<endl;
    cout<<"*evaluation for the end of Year 3.\t\t\t*"<<endl<<endl;
    cout<<"========================================================="<<endl<<endl;
    cout<<"Starting Time: Thu Apr 05 23:59:08 CAT 2022"<<endl;
    cout<<"Need a help? Type 'help' then press Enter key."<<endl<<endl;
    cout<<"Console > ";

    getline(cin, option);

    transform(option.begin(), option.end(), option.begin(), ::toupper);

    if(option == "EXIT") {
        cout<<"Thanks for using our services!"<<endl;
        exit(0);
    } else if(option == "HELP"){
        system("cls");
        user_manual();
    } else if (option.rfind("ADD", 0) == 0) {
        vector<string> option_location;
        tokenize_string(option, ' ', option_location);

        if(option_location.size()==2) {
            string location = option_location.at(1);
            add_location(location);

            cout<<"location "<<location<<" is successfully added!"<<endl;
        } else {
			cout<<"Command doesn't exist"<<endl;
		}
    } else if(option == "LIST LOCATIONS"){
        list_locations();
        
	} else if(option == "LIST DISEASES"){
        list_diseases();
        
	} else if(option.rfind("DELETE", 0) == 0){
         vector<string> option_location;
        tokenize_string(option, ' ', option_location);

        string location = option_location.at(1);
        delete_location(location);
        
    }  else if(option.rfind("RECORD", 0) == 0){
         vector<string> option_location;
        tokenize_string(option, ' ', option_location);
        
        
		if(option_location.size()==4) {
	        string location = option_location.at(1);
	        string disease = option_location.at(2);
            if (is_string_an_integer(option_location.at(3)))
            {
                int case_number = stoi(option_location.at(3));
                add_disease(location, disease, case_number);
            } else
            {
                cout<<"Cases must be in numbers"<<endl;
            }
            
            
		} else {
			cout<<"Command doesn't exist"<<endl;
		}

	} else if(option.rfind("WHERE", 0) == 0){
        vector<string> option_disease;
        tokenize_string(option, ' ', option_disease);

        string disease = option_disease.at(1);
        find_disease_location(disease);
    } else if(option.rfind("CASES", 0) == 0){
        vector<string> option_disease;
        tokenize_string(option, ' ', option_disease);
        if (option_disease.size()==2)
        {
            string disease = option_disease.at(1);
            find_total_cases(disease);
        } else if (option_disease.size()==3)
        {
            string location = option_disease.at(1);
            string disease = option_disease.at(2);
            find_location_cases(location, disease);
        } else {
			cout<<"Command doesn't exist"<<endl;
		}
    } else if(option.rfind("SUMMARY", 0) == 0) {
        summary();
    } else {
		cout<<"Command doesn't exist"<<endl;
	}
    
} while (option!="exit");

	return 0;
}
