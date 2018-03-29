// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5)
    : m_numItems(0), m_arrSize(100)
    {
        if (maxLoadFactor <= 0)
            m_maxLoad = 0.5;
        else if (maxLoadFactor > 2)
            m_maxLoad = 2.0;
        else
            m_maxLoad = maxLoadFactor;
        
        m_hashTable = new Bucket*[100];
        for (int i = 0; i < 100; i++) {
            m_hashTable[i] = nullptr;
        }
    }
    
    ~MyHash()
    {
        for (int i = 0; i < m_arrSize; i++) {
            clearBuckets(m_hashTable[i]);
        }
        delete[] m_hashTable;
    }
    
    void reset() {
        // free memory in current hashtable
        for (int i = 0; i < m_arrSize; i++) {
            clearBuckets(m_hashTable[i]);
        }
        delete[] m_hashTable;
        // create new empty hashtable
        m_hashTable = new Bucket*[100];
        for (int i = 0; i < 100; i++) {
            m_hashTable[i] = nullptr;
        }
        m_numItems = 0;
        m_arrSize = 100;
    }
    
    void associate(const KeyType& key, const ValueType& value) {
        ValueType* comp = find(key);
        // found the key
        if (comp != nullptr) {
            if (*comp != value) {
                *comp = value;
            }
            return;
        }
        // comp is nullptr (key not found!)
        else {
            // load limit surpassed
            if ((m_numItems + 1.0) / m_arrSize > m_maxLoad) {
                // create new hash table with double the size
                int prevSize = m_arrSize;
                m_arrSize *= 2;
                Bucket** tempArr = new Bucket*[m_arrSize];
                for (int i = 0; i < m_arrSize; i++) {
                    tempArr[i] = nullptr;
                }
                
                // fill new hash table
                for (int i = 0; i < prevSize; i++) {
                    if (m_hashTable[i] == nullptr)
                        continue;
                    else {
                        Bucket* temp = m_hashTable[i];
                        // loop throughout bucket & add the linked list in
                        while (temp != nullptr) {
                            insertBucket(tempArr, temp->m_key, temp->m_value);
                            temp = temp->m_next;
                        }
                    }
                }
                // clear prev hash table
                for (int i = 0; i < prevSize; i++) {
                    clearBuckets(m_hashTable[i]);
                }
                delete[] m_hashTable;
                
                // set new hash table
                m_hashTable = tempArr;
                
                insertBucket(m_hashTable, key, value);
                m_numItems++;
                
            }
            // insert into proper bucket
            else {
                insertBucket(m_hashTable, key, value);
                m_numItems++;
            }
        }
    }
    
    int getNumItems() const {
        return m_numItems;
    }
    
    double getLoadFactor() const {
        return m_numItems * 1.0 / m_arrSize;
    }
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const {
        unsigned int bucketNum = getBucketNum(key);
        Bucket* temp = m_hashTable[bucketNum];
        while (temp != nullptr) {
            if (temp->m_key == key)
                return &(temp->m_value);
            temp = temp->m_next;
        }
        return nullptr;
    }
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    struct Bucket
    {
        KeyType   m_key;
        ValueType m_value;
        Bucket*     m_next;
    };
    Bucket** m_hashTable;
    int m_numItems;
    int m_arrSize;
    double m_maxLoad;
    
    void clearBuckets(Bucket* bucket) {
        if (bucket == nullptr)
            return;
        clearBuckets(bucket->m_next);
        delete bucket;
    }
    
    unsigned int getBucketNum(const KeyType& Key) const {
        unsigned int hash(const KeyType& k);
        unsigned int h = hash(Key);
        return h % m_arrSize;
    }
    
    // THIS FUNCTION DOES NOT INCREMENT M_NUMITEMS
    void insertBucket(Bucket** hashTable, KeyType key, ValueType value) {
        // create new item to be inserted
        Bucket* toInsert = new Bucket;
        toInsert->m_key = key;
        toInsert->m_value = value;
        toInsert->m_next = nullptr;
        
        // get the bucket to which it should be inserted
        unsigned int bucketNum = getBucketNum(key);
        
        // no linked list yet
        if (hashTable[bucketNum] == nullptr)
            hashTable[bucketNum] = toInsert;
        // one or more items in bucket already
        else if (hashTable[bucketNum] != nullptr){
            Bucket* temp = hashTable[bucketNum];
            while (temp != nullptr && temp->m_next != nullptr) {
                temp = temp->m_next;
            }
            temp->m_next = toInsert;
        }
    }
};

