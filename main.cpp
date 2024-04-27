#include <iostream>
#include <vector>
#include <map>
using namespace std;

class InMemoryDB
{
private:
    map<string, int> transactions;
    map<string, bool> transactionCommitState;
    bool isTransactionActive = false;
    int originalVal = 0;
    string currentKey = "";
    int currentVal = 0;
public:
    int get(string key);
    void put(string key, int val);
    void begin_transaction();
    void commit();
    void rollback();
};

int InMemoryDB::get(string key)
{
    if (transactions.find(key) == transactions.end())
        cout << "NULL" << endl;
    else if (!transactionCommitState[key])
        cout << "NULL" << endl;
    else
        return transactions[key];
    return 0;
}

void InMemoryDB::put(string key, int val)
{
    if (isTransactionActive) {
        if (transactions.find(key) == transactions.end())
        {
            originalVal = val;
        }
        transactions[key] = val;
        transactionCommitState[key] = false;
        currentKey = key;
        currentVal = val;
    }
    else
        cout << "ERROR: A transaction is not in progress_2" << endl;
}
void InMemoryDB::begin_transaction()
{
    isTransactionActive = true;
}
void InMemoryDB::commit()
{
    if (isTransactionActive)
    {
        transactionCommitState[currentKey] = true;
        isTransactionActive = false;
    }
    else
        cout << "ERROR: A transaction is not in progress_3" << endl;
}
void InMemoryDB::rollback()
{
    if (isTransactionActive)
    {
        transactions.erase(currentKey);
        transactionCommitState.erase(currentKey);
    }
    else
        cout << "ERROR: A transaction is not in progress_4" << endl;
}

int main() {
    InMemoryDB inmemoryDB;
    int result = 0;

// should return null, because A doesn’t exist in the DB yet
    result = inmemoryDB.get("A");

// should throw an error because a transaction is not in progress
    inmemoryDB.put("A", 5);

// starts a new transaction
    inmemoryDB.begin_transaction();

// set’s value of A to 5, but its not committed yet
    inmemoryDB.put("A", 5);

// should return null, because updates to A are not committed yet
    result = inmemoryDB.get("A");

// update A’s value to 6 within the transaction
    inmemoryDB.put("A", 6);

// commits the open transaction
    inmemoryDB.commit();

    // should return 6, that was the last value of A to be committed
    result = inmemoryDB.get("A");
    cout << result << endl;

// throws an error, because there is no open transaction
    inmemoryDB.commit();

// throws an error because there is no ongoing transaction
    inmemoryDB.rollback();

// should return null because B does not exist in the database
    inmemoryDB.get("B");

// starts a new transaction
    inmemoryDB.begin_transaction();

// Set key B’s value to 10 within the transaction
    inmemoryDB.put("B", 10);

// Rollback the transaction - revert any changes made to B
    inmemoryDB.rollback();

// Should return null because changes to B were rolled back
    inmemoryDB.get("B");

    return 0;
}
