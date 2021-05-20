#include<iostream>
#include<vector>
#include<string>
using namespace std;
vector<bool> safety_algorithm(vector<int> available, vector<vector<int>> max, vector<vector<int>> allocation, vector<vector<int>> &need, 
							  int m, int n, vector<string> &processes, vector<bool> finish) {
	vector<int> work(m);
	work = available;
	int i = 0;
	int count = 0;
	int index = 0;
	bool is_need_less_than_work;
	while (true) {
		is_need_less_than_work = true;
		for (int j = 0; j < m; j++) {
			if (need[i][j] > work[j])
				is_need_less_than_work = false;
		}
		if (!finish[i] && is_need_less_than_work) {
			finish[i] = true;
			processes[index] = "P" + to_string(i);
			for (int j = 0; j < m; j++)
				work[j] += allocation[i][j];
			count++;
			index++;
		}
		if (i == (n - 1) && count != 0)
			count = 0;
		else if (i == (n - 1) && count == 0) {
			return finish;
		}
		i = (i + 1) % n;
	}
}
bool request_algorithm(vector<int> available, vector<vector<int>> max, vector<vector<int>> allocation, vector<vector<int>> &need, 
					   int m, int n, bool &error, vector<string> &processes, int number_process_to_modify) {
	vector<int> request(m);
	vector<bool> finish(n);
	bool is_zero = true;
	bool is_request_less_than_available = true;
	cout << "Enter the value of process to modify: ";
	for (int i = 0; i < m; i++)
		cin >> request[i];
	
	for (int i = 0; i < m; i++) {
		if (request[i] > need[number_process_to_modify][i]) {
			error = true;
			return false;
		}
	}

	for (int i = 0; i < m; i++) {
		if (request[i] > available[i])
			is_request_less_than_available = false;
	}

	if (is_request_less_than_available) {
		for (int i = 0; i < m; i++) {
			available[i] -= request[i];
			allocation[number_process_to_modify][i] += request[i];
			need[number_process_to_modify][i] -= request[i];
			if (need[number_process_to_modify][i] != 0) {
				is_zero = false;
			}
		}
		if (is_zero)
			finish[number_process_to_modify] = true;
		finish = safety_algorithm(available, max, allocation, need, m, n, processes, finish);
	}
	else
		return false;

	for (int i = 0; i < n; i++) {
		if (!finish[i]) {
			return false;
		}
	}
	return true;
}

void take_input() {
	//*********************take n and m************
	//n = nubmer_of_process     m = number_of_resources
	int n, m;
	cout << "Please enter number of processes and resources seprated by space: ";
	cin >> n >> m;
	
	if (n <= 0 || m <= 0) {
		cout << "Invalid Input." << endl;
		return take_input();
	}

	vector<int> available(m);
	vector<vector<int>> max(n, vector<int>(m, 0));
	vector<vector<int>> allocation(n, vector<int>(m, 0));
	vector<vector<int>> need(n, vector<int>(m, 0));
	vector<bool> finish(n);
	vector<string> processes(n);
	int number_process_to_modify;
	int algorithm_needed;
	int another_transaction;
	bool flag = true;
	bool request_alg = false;
	bool error = false;

	//  *******************take input matricess*****************
	char c;
	cout << "\nEnter Allocation Matrix:" << endl;
	cout << "    ";
	for (int i = 0; i < m; i++) {
		c = 'A' + i;
		cout << c << " ";
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << "P" << i << ": ";
		for (int j = 0; j < m; j++) {
			cin >> allocation[i][j];
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (allocation[i][j] < 0) {
				cout << "Invalid Input." << endl;
				return;
			}
		}
	}
	cout << "\nEnter Max Matrix:" << endl;
	cout << "    ";
	for (int i = 0; i < m; i++) {
		c = 'A' + i;
		cout << c << " ";
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << "P" << i << ": ";
		for (int j = 0; j < m; j++) {
			cin >> max[i][j];
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (max[i][j] < 0) {
				cout << "Invalid Input." << endl;
				return;
			}
			if (allocation[i][j] > max[i][j]) {
				cout << "Allocation " << "can't be greater than Max for Process " << i <<"." << endl;
				return;
			}
		}
	}
	cout << "\nEnter Available Matrix:" << endl;
	for (int i = 0; i < m; i++) {
		c = 'A' + i;
		cout << c << " ";
	}
	cout << endl;
	for (int i = 0; i < m; i++) {
		cin >> available[i];
	}
	for (int i = 0; i < m; i++) {
		if (available[i] < 0) {
			cout << "Invalid Input." << endl;
			return;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}

	// ********************Need Matrix****************************
	cout << "\nNeed Matrix: " << endl;
	cout << "    ";
	for (int i = 0; i < m; i++) {
		c = 'A' + i;
		cout << c << " ";
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << "P" << i << ": ";
		for (int j = 0; j < m; j++) {
			cout << need[i][j] << " ";
		}
		cout << endl;
	}

	// ******************choose needed algrothim************************

	cout << "\nPlease choose the algorithm needed" << endl << "0: The safety algorithm" << endl << "1: The request algorithm" << endl;
	cin >> algorithm_needed;

	if (algorithm_needed == 0)
		finish = safety_algorithm(available, max, allocation, need, m, n, processes, finish);
	else if (algorithm_needed == 1) {
		cout << "\nEnter number of process you need to modify: ";
		cin >> number_process_to_modify;
		request_alg = request_algorithm(available, max, allocation, need, m, n, error, processes, number_process_to_modify);
	}
	else
		return take_input();
	for (int i = 0; i < n; i++) {
		if (!finish[i]) {
			flag = false;
			break;
		}
	}
	if (error)
		cout << "\nError, Request can't be greater than need." << endl;
	else if (flag && algorithm_needed == 0) {
		cout << "\nYes, Safe state ";
		cout << "<";
		for (int i = 0; i < n; i++) {
			cout << processes[i];
			if (i != (n - 1))
				cout << ", ";
		}
		cout << ">" << endl;
	}
	else if (request_alg && algorithm_needed == 1) {
		cout << "\nYes request can be granted with safe state, Safe state  ";
		cout << "<" << "P" << number_process_to_modify << "req, ";
		for (int i = 0; i < n; i++) {
			cout << processes[i];
			if (i != (n - 1))
				if (processes[i + 1] != "")
					cout << ", ";
		}
		cout << ">" << endl;
	}
	else if (!request_alg && algorithm_needed == 1)
		cout << "\nNot Granted." << endl;
	else
		cout << "\nNot Safe. " << endl;
	cout << "\nPress 0 if you need another transaction and press any key for end: ";
	cin >> another_transaction;
	cout << endl;
	if (another_transaction == 0)
		return take_input();
	else
		return;

}
int main()
{
	take_input();
	system("Pause");
	return 0;
}