#include "TableType.h"
#include <unordered_map>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

///// TENEG PETKA
///Harnaaaaaaaaa
enum class Tag { String, Double, Int, Bool };

struct Less {
  public:
  	Less(const TableType& a, int q) : value(a), index(q){} 
    bool operator() (const vector<TableType>& z) const
    { return z[index] < value; }
  private:
  	TableType value;
  	int index;
};

struct Greater {
  public:
  	Greater(const TableType& a, int q) : value(a), index(q){} 
    bool operator() (const vector<TableType>& z) const
    { return z[index] > value; }
  private:
  	TableType value;
  	int index;
};

struct Equal {
 public:
  	Equal(const TableType& a, int q) : value(a), index(q){} 
    bool operator() (const vector<TableType>& z) const
    { return z[index] == value; }
 private:
  	TableType value;
  	int index;
};


struct table{
	vector<vector<TableType> > box;
	vector<string> col_type;
	unordered_map<string,int> colname_index;
	unordered_map<TableType,vector<int>> hash_map;
	map<TableType,vector<int>> bst;
	string col_name;
};
/*
for (auto c : mykeys)
    myMap.emplace(c, 0);
    *///
void gen_fn(unordered_map<string,table>& database, const string& index_type, const string& tableName,
	const string& colname){
	table &some = database[tableName];
	some.col_name = colname;
	some.hash_map.clear();
	some.bst.clear();
	int Col_Id = some.colname_index[colname];
	if(index_type == "hash"){
		for(unsigned int i =0; i < some.box.size(); ++i){
			const auto& key = some.box[i][Col_Id];
			some.hash_map[key].push_back(i);
		}
	}
	else{
		for(unsigned int i =0; i < some.box.size(); ++i){
			const auto& key = some.box[i][Col_Id];
			some.bst[key].push_back(i);
		}
	}
}

bool tablename_exist(unordered_map<string,table>& database, const string& table){
	auto it = database.find(table);
	if(it == database.end()){
		return false;
	} 
	else return true;
}

bool colname_exist(table &temp, const string& colname){
	auto it = temp.colname_index.find(colname);
	if(it == temp.colname_index.end()){
		return false;
	}
	else return true;
}

void createTable(unordered_map<string,table>& t){
	string tableName;
	int num;
	string coltype, junk;
	string colname; // necessary temp variables
	cin>> tableName;
	cin>> num;
	if(tablename_exist(t, tableName)){
		cout << "Error: Cannot create already existing table "<< tableName <<"\n";
		getline(cin,junk);
		return;
	}
	//table new_table;//initializing the temp class
	
	table ptr;
	t.emplace(tableName, ptr); //create hash from tablename to table
	//new_table.box.resize(num);
	//ptr->col_type.resize(num);
	for(int i=0; i< num; ++i){
		cin >> coltype;
		ptr.col_type.push_back(coltype); 
	}
	cout << "New table " << tableName <<" with data ";
	for(int i=0; i< num; ++i){
		cin >> colname;
		cout << colname << " ";
		ptr.colname_index.emplace(colname,i); 
	}
	cout<<"created\n";
}

void insertTable(unordered_map<string,table>& t){
	string junk;
	string tableName;
	int num;
	string temp;
	string str;
	int it;
	bool bl;
	double db;
	vector<TableType> vec;
	cin>> junk;
	cin>>tableName;
	cin>>num; cin>>junk;
	if(!tablename_exist(t, tableName)){
		cout << "Error: "<< tableName << " does not name a table in the database\n";
		getline(cin,junk);
		for(int k=0; k < num; ++k){
			getline(cin,junk);
		}
		return;
	}
	table &some = t[tableName];
	int prev_size = some.box.size();
	for(int i=0; i<num; ++i){
		vec.clear();
		for(unsigned int j=0; j<some.col_type.size(); ++j){
			temp = some.col_type[j];
			switch(temp[0]){
				case 's': {
					cin >> str;
					//cout <<"the string is " << str << endl;
					vec.push_back(TableType(str));
					break;
				}
				case 'd': {
					cin >> db;
					//cout <<"the doouble is " << db << endl;
					vec.push_back(TableType(db));
					break;
				}
				case 'i': {
					cin >> it;
					//cout <<"the int is " << it << endl;
					vec.push_back(TableType(it));
					break;

				}
				case 'b': {
					cin >> bl;
					//cout <<"the bool is " << bl << endl;
					vec.push_back(TableType(bl));
					break;
				}
			}
		}
		some.box.push_back(vec);
	}
	//TODO rebuild index


	if(some.hash_map.empty() && some.bst.empty()){
			//haven't generated the index yet
			//gen_fn(t,"bst",tableName,colname);
		}
		else if(some.bst.empty()){
			
			gen_fn(t,"hash",tableName,some.col_name);
		}
		else if(some.hash_map.empty()){
			
			gen_fn(t,"bst",tableName,some.col_name);
		}


	cout << "Added " << num <<" rows to " << tableName << " from position " << prev_size <<" to "
	<< prev_size+num-1 << "\n";
}


int del_fn(vector<vector<TableType>> &vec, char op, const TableType& ta, int index){
	int del_NUm = 0;
		switch(op){
			case '<': {
				Less fun(ta,index);
				auto iter_end = vec.end();
				auto iter = remove_if(vec.begin(),vec.end(), fun);
				del_NUm = iter_end - iter;
				vec.erase(iter,iter_end);
				//return del_NUm;
				break;
		    }
			case '>':{
				Greater fun(ta,index);
				auto iter_end = vec.end();
				auto iter = remove_if(vec.begin(),vec.end(), fun);
				del_NUm = iter_end - iter;
				vec.erase(iter,iter_end);
				//return del_NUm;
				break;
			}
			case '=':{
				Equal fun(ta,index);
				auto iter_end = vec.end();
				auto iter = remove_if(vec.begin(),vec.end(), fun);
				del_NUm = iter_end - iter;
				vec.erase(iter,iter_end);
				//return del_NUm;
				break;
			}
	
	}
	return del_NUm;
}

void deleteTable(unordered_map<string,table>& t){
	string junk, temp;
	string tableName, colname;
	int del_Num = 0;
	char op;// string value;
	int index;
	cin >> junk; // takes FROM
	cin>> tableName; cin >> junk;//takes where
	if(!tablename_exist(t, tableName)){
		cout << "Error: "<< tableName << " does not name a table in the database\n";
		getline(cin,junk);
		return;
	}
	cin >> colname; cin >> op;
	table &some = t[tableName];
	if(!colname_exist(some, colname)){
		cout << "Error: "<< colname << " does not name a column in "<< tableName << "\n";
		getline(cin,junk);
		return;
	}
	index = some.colname_index[colname];
	 temp = some.col_type[index];
			switch(temp[0]){
				case 's': {
					string value;
					cin >> value; 
					TableType tt{value};
					del_Num = del_fn(some.box, op, tt,index);
					//cout <<"the string is " << value << endl;
					break;
				}
				case 'd': {
					double value;
					cin >> value;
					TableType tt{value};
					del_Num = del_fn(some.box, op, tt,index);
					
					break;
				}
				case 'i': {
					int value;
					cin >> value;
					TableType tt{value};
					del_Num = del_fn(some.box, op, tt,index);
					
					break;

				}
				case 'b': {
					bool value;
					cin >> value;
					TableType tt{value};
					del_Num = del_fn(some.box, op, tt,index);
					
					break;
				}
		}
		// TODO rebuild index
		if(some.hash_map.empty() && some.bst.empty()){
			//haven't generated the index yet
			//gen_fn(t,"bst",tableName,colname);
		}
		else if(some.bst.empty()){
			
			gen_fn(t,"hash",tableName,colname);
		}
		else if(some.hash_map.empty()){
			
			gen_fn(t,"bst",tableName,colname);
		}

		cout <<"Deleted " <<del_Num << " rows from "
		<< tableName << "\n";
}




void generate(unordered_map<string,table>& database){
	string junk, tableName, indextype, colname;
	string type;
	cin >> junk;
	cin>> tableName;
	if(!tablename_exist(database, tableName)){
		cout << "Error: "<< tableName << " does not name a table in the database\n";
		getline(cin,junk);
		return;
	}
	cin >> indextype;
	cin >> junk; cin >> junk;
	cin >> colname;
	if(!colname_exist(database[tableName], colname)){
		cout << "Error: "<< colname << " does not name a column in "<< tableName << "\n";
		getline(cin,junk);
		return;
	}
	
	gen_fn(database, indextype, tableName, colname);
	cout << "Created " <<indextype <<" index for table "<<tableName
	<<" on column "<<colname << "\n";
}

template<typename Functor>
void print_second_helper(unordered_map<string,table>& database, Functor f, const string& tableName, int &num,
	const vector<int> &col_print, bool quiet_mode){
	int row_val = 0;
	table &some = database[tableName];
		for(unsigned int i =0 ; i < some.box.size(); ++i){
			if(f(some.box[i])){
				num++;
				if(quiet_mode){
				}
				else{
					for(auto it = col_print.begin(); it != col_print.end(); ++it){
						row_val = *it;
						cout<< some.box[i][row_val] << " ";
					}
					cout<<"\n";
					}
				
				}
		}
}

bool print_bst_helper(unordered_map<string,table>& database, const TableType& tt, const string& tableName, int &num,
	const vector<int> &col_print, bool quiet_mode, const string& colname, char ch){
	int row_val = 0;
	table &some = database[tableName]; 
	if(some.col_name == colname){
		if(ch == '<'){
		for(auto it = some.bst.begin(); it != some.bst.lower_bound(tt); ++it){
			if (it == some.bst.end()) {
					return true;
				// not found
			}
			for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
				num++;
				if(quiet_mode){}
				else{
					for(auto iter = col_print.begin(); iter != col_print.end(); ++iter){
						row_val = *iter;
						cout<< some.box[*jt][row_val] << " ";
						}
					}
				}
					if(quiet_mode){}
					else{
						cout<<"\n";
				}	
			}
			return true;
		}
		else if(ch == '>'){
		for(auto it = some.bst.upper_bound(tt); it != some.bst.end(); ++it){
			if (it == some.bst.end()) {
					return true;
				// not found
			}
			for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
				num++;
				if(quiet_mode){}
				else{
					for(auto iter = col_print.begin(); iter != col_print.end(); ++iter){
						row_val = *iter;
						cout<< some.box[*jt][row_val] << " ";
						}
					}
				}
					if(quiet_mode){}
					else{
						cout<<"\n";
				}	
			}
			return true;
		}
		else{
			auto it = some.bst.find(tt);
			if (it == some.bst.end()) {
					return true;
				// not found
			}
			for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
				num++;
				if(quiet_mode){}
				else{
					for(auto iter = col_print.begin(); iter != col_print.end(); ++iter){
						row_val = *iter;
						cout<< some.box[*jt][row_val] << " ";
						}
					}
				}
					if(quiet_mode){}
					else{
						cout<<"\n";
				}	
			
			return true;
		}
	}
		return false;
}
void print_helper(unordered_map<string,table>& database, char op, const TableType& ta, int index,
	const vector<int> &col_print, const string& tableName, bool quiet_mode, const string& colname){
		int num_print = 0;
		table &some = database[tableName];
		switch(op){
			case '<': {
				Less fun(ta,index);
				//when both hash and bst empty
					if(some.hash_map.empty() && some.bst.empty()){
					//haven't generated the index yet
						print_second_helper(database, fun, tableName,num_print,col_print,quiet_mode);
				}
					else if(some.bst.empty()){
						print_second_helper(database, fun, tableName,num_print,col_print,quiet_mode);
				}
					else if(some.hash_map.empty()){
						if(!print_bst_helper(database, ta, tableName,num_print,col_print,quiet_mode,colname,'<')){
								print_second_helper(database, fun, tableName,num_print,col_print,quiet_mode);
						}
				}
			
				//return del_NUm;
				break;
		    }
			case '>':{
				Greater fun(ta,index);
				//when both hash and bst empty
					if(some.hash_map.empty() && some.bst.empty()){
					//haven't generated the index yet
						print_second_helper(database,fun,tableName,num_print,col_print,quiet_mode);
				}
					else if(some.bst.empty()){
						print_second_helper(database,fun,tableName,num_print,col_print,quiet_mode);
				}
					else if(some.hash_map.empty()){
						if(!print_bst_helper(database,ta,tableName,num_print,col_print,quiet_mode,colname,'>')){
							print_second_helper(database,fun,tableName,num_print,col_print,quiet_mode);
						}
				}
			
				//return del_NUm; 
				break;
			}
			case '=':{
				Equal fun(ta,index);
						//when both hash and bst empty
					if(some.hash_map.empty() && some.bst.empty()){
					//haven't generated the index yet
						print_second_helper(database,fun,tableName,num_print,col_print,quiet_mode);
				}
					else if(some.bst.empty()){
						print_second_helper(database,fun,tableName,num_print,col_print,quiet_mode);
				}
					else if(some.hash_map.empty()){
						if(!print_bst_helper(database,ta,tableName,num_print,col_print,quiet_mode,colname,'=')){
							print_second_helper(database,fun,tableName,num_print,col_print,quiet_mode);
						}
				}
			
				//return del_NUm;
				break;
			}
		
	}
	cout << "Printed " << num_print << " matching rows from " << tableName <<"\n"; 
	return;
}

void printTable(unordered_map<string,table>& database, bool quiet_mode){
	string junk, colname ,tableName, option;
	string type_col;
	ostringstream os;
	char op;
	int num, row_val;
	int Col_Id;
	vector<int> col_print;
	cin >> junk; // takes the FROM
	cin >> tableName;
	table &some = database[tableName];
	if(!tablename_exist(database, tableName)){
		cout << "Error: "<< tableName << " does not name a table in the database\n";
		getline(cin,junk);
		return;
	}
	cin >> num;

	for(int i = 0 ; i < num; ++i){
		cin >> colname; // reads all the colnames
		if(!colname_exist(some, colname)){
		cout << "Error: "<< colname << " does not name a column in "<< tableName << "\n";
		getline(cin,junk);
		return;
		}
		Col_Id = some.colname_index[colname];
		col_print.push_back(Col_Id);
		os << colname << " ";
	}
	os <<"\n";
	cin>> option; // can be wither WHERE or ALL
	if(option == "ALL"){//print all Function 
		if(quiet_mode){

		}
		else{
			cout << os.str();
			for(unsigned int i = 0; i < some.box.size(); ++i){
				for(int j = 0; j < num; ++j){
					row_val = col_print[j];
					cout << some.box[i][row_val] << " ";
				}
				cout<<"\n";
			}
		}
		cout << "Printed " << some.box.size() << " matching rows from " << tableName <<"\n";
	}
	else{
		cin >> colname;
		if(!colname_exist(some, colname)){
		cout << "Error: "<< colname << " does not name a column in "<< tableName << "\n";
		getline(cin,junk);
		return;
		}
		if(quiet_mode){

		}
		else{	
			cout << os.str();
		}
		cin >> op;
		Col_Id = some.colname_index[colname];
		type_col = some.col_type[Col_Id];
			switch(type_col[0]){
				case 's': {
					string value;
					cin >> value; 
					TableType tt{value};
					print_helper(database,op,tt,Col_Id,col_print,tableName,quiet_mode,colname);
					//cout <<"the string is " << value << endl;
					break;
				}
				case 'd': {
					double value;
					cin >> value;
					TableType tt{value};
					print_helper(database,op,tt,Col_Id,col_print,tableName,quiet_mode,colname);
					break;
				}
				case 'i': {
					int value;
					cin >> value;
					TableType tt{value};
					print_helper(database,op,tt,Col_Id,col_print,tableName,quiet_mode,colname);
					break;

				}
				case 'b': {
					bool value;
					cin >> value;
					TableType tt{value};
					print_helper(database,op,tt,Col_Id,col_print,tableName,quiet_mode,colname);
					break;
				}
		}
	}	
}


void join(unordered_map<string,table>& database, bool quiet_mode){
	string junk, tableName1, tableName2;
	string colname1, colname2, print_column;
	vector<string> names;
	vector<int> type;
	int num, sel, in1, choice, index, count=0;
	cin >> tableName1;
	table &some1 = database[tableName1];
	if(!tablename_exist(database, tableName1)){
		cout << "Error: "<< tableName1 << " does not name a table in the database\n";
		getline(cin,junk);
		return;
	}	
	cin >> junk;
	cin >> tableName2;
	table &some2 = database[tableName2];
	if(!tablename_exist(database, tableName2)){
		cout << "Error: "<< tableName2 << " does not name a table in the database\n";
		getline(cin,junk);
		return;
	}	
	cin >> junk;
	cin >> colname1;
	if(!colname_exist(some1, colname1)){
		cout << "Error: "<< colname1 << " does not name a column in "<< tableName1 << "\n";
		getline(cin,junk);
		return;
	}
	cin >> junk;
	cin >> colname2;
	if(!colname_exist(some2, colname2)){
		cout << "Error: "<< colname2 << " does not name a column in "<< tableName2 << "\n";
		getline(cin,junk);
		return;
	}
	cin >> junk; cin >> junk;
	cin >> num;
	
	
	//compare the coltypes, cells ?
	//unordered_map<string,table*>& database, string index_type, string tableName,
	string colname;
	in1 = some1.colname_index[colname1]; // index of row (table1)
	//in2 = database[tableName2]->colname_index[colname2]; // index of row (table2)

	for(int i =0; i < num ; ++i){//reads in the input and stores in the vector
		cin >> print_column;
		names.push_back(print_column);
		cin >> sel;
		type.push_back(sel);
		if(sel == 1){
			if(!colname_exist(some1, print_column)){
			cout << "Error: "<< print_column << " does not name a column in "<< tableName1 << "\n";
			getline(cin,junk);
			return;
			}
		}
		else {
			if(!colname_exist(some2, print_column)){
			cout << "Error: "<< print_column << " does not name a column in "<< tableName2 << "\n";
			getline(cin,junk);
			return;
			}
		}
		if(quiet_mode){
		}
		else{
			cout << print_column << " ";
		}
	}
	if(quiet_mode){
	}
	else{
		cout << "\n";
	}
	int j = 0;
	gen_fn(database, "hash", tableName2, colname2);
	for(unsigned int i= 0; i < some1.box.size(); ++i){
		auto key = some1.box[i][in1];
		auto got = some2.hash_map.find(key);
		if(got != some2.hash_map.end()){
		vector<int> temp(some2.hash_map[key]); 
		for(auto it = temp.begin(); it != temp.end(); ++it){// table1 have a match
			count++;
			if(quiet_mode){}
			else{
				for( j = 0; j < num; ++j){ // which row to print from 
					choice = type[j]; //decide from which table 
					print_column = names[j]; // which column name to choose 
					if(choice == 1 ){
						index = some1.colname_index[print_column];
						cout << some1.box[i][index] <<" ";
						}
					else {
						index = some2.colname_index[print_column];
						cout << some2.box[*it][index] <<" ";
						}
			  		}
			  		cout << "\n";
			  	}						
			}
		}
	}
	cout << "Printed " << count << " rows from joining "<< tableName1 <<" to "
	 << tableName2 << "\n";
}


int main(int argc, char *argv[]){
		ios_base::sync_with_stdio(false);
		cin >> boolalpha;
		cout << boolalpha;
	bool quiet_mode = false;
	if(argc == 2){
		if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help")==0){
			cout<<"Print some helpful messages to the output\n";
		}
		else if(strcmp(argv[1],"-q") == 0 || strcmp(argv[1],"--quiet") == 0){
			quiet_mode = true;
		}
	}
	if(quiet_mode){
		int i = 1;
		i++;
		//cout<<"Do things when quiet mode is neccessary\n";
	}
	
	string input;
	string comment;
	string tableName;
	unordered_map<string, table> my_tables;
	
	while(cin>>input){
		switch(input[0]){
			case 'C':
				cout<<"% ";
				createTable(my_tables);
				break;
			case 'I':
				cout<<"% ";
				insertTable(my_tables);
				//cout<<"insert mode ended\n";
				break;
			case 'D':
				cout<<"% ";
				//cout<< "delete mode\n";
				deleteTable(my_tables);
				break;
			case 'G':
				cout<<"% ";
				//cout<<"generate moed\n";
				generate(my_tables);
				break;
			case 'P':
				cout<<"% ";
				printTable(my_tables,quiet_mode);
				//cout << "print mode\n";
				break;
			case 'J':
				cout<<"% ";
				join(my_tables,quiet_mode);
				//cout<< "join mode\n";
				break;
			case 'R':
				cout<<"% ";
				cin >> tableName;
				if(!tablename_exist(my_tables, tableName)){
				cout << "Error: "<< tableName << " does not name a table in the database\n";
				getline(cin,comment);
				break;
				}
				//delete my_tables[tableName];
				my_tables.erase(tableName);
				cout << "Table " << tableName << " deleted\n";
				//cout <<"remove mode\n";
				break;
			case '#':
				cout<<"% ";
				getline(cin, comment);
				//is the comment line always be one line or multiple lines?
				//how are we handling error cases like not existing tablenames and column names
				//exit(0);
				//cout efficiency
				break;
			case 'Q':
				cout<<"% ";
				cout <<"Thanks for being silly!\n";
				return 0;
				break;
			default:
				cout << "Error: unrecognized command\n";
				getline(cin, comment);
				break;
		}
	}
	return 0;
}
