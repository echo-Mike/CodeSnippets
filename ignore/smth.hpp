#ifdef USLES

template <class T, class A = std::allocator<T> >
class X {
public:
    typedef A allocator_type;
    typedef typename A::value_type value_type; 
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;

    class iterator { 
    public:
        typedef typename A::difference_type difference_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::pointer pointer;
        typedef std::random_access_iterator_tag iterator_category; //or another tag

        iterator();
        iterator(const iterator&);
        ~iterator();

        iterator& operator=(const iterator&);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
        bool operator<(const iterator&) const; //optional
        bool operator>(const iterator&) const; //optional
        bool operator<=(const iterator&) const; //optional
        bool operator>=(const iterator&) const; //optional

        iterator& operator++();
        iterator operator++(int); //optional
        iterator& operator--(); //optional
        iterator operator--(int); //optional
        iterator& operator+=(size_type); //optional
        iterator operator+(size_type) const; //optional
        friend iterator operator+(size_type, const iterator&); //optional
        iterator& operator-=(size_type); //optional            
        iterator operator-(size_type) const; //optional
        difference_type operator-(iterator) const; //optional

        reference operator*() const;
        pointer operator->() const;
        reference operator[](size_type) const; //optional
    };
    class const_iterator {
    public:
        typedef typename A::difference_type difference_type;
        typedef typename A::value_type value_type;
        typedef typename const A::reference reference;
        typedef typename const A::pointer pointer;
        typedef std::random_access_iterator_tag iterator_category; //or another tag

        const_iterator ();
        const_iterator (const const_iterator&);
        const_iterator (const iterator&);
        ~const_iterator();

        const_iterator& operator=(const const_iterator&);
        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator&) const;
        bool operator<(const const_iterator&) const; //optional
        bool operator>(const const_iterator&) const; //optional
        bool operator<=(const const_iterator&) const; //optional
        bool operator>=(const const_iterator&) const; //optional

        const_iterator& operator++();
        const_iterator operator++(int); //optional
        const_iterator& operator--(); //optional
        const_iterator operator--(int); //optional
        const_iterator& operator+=(size_type); //optional
        const_iterator operator+(size_type) const; //optional
        friend const_iterator operator+(size_type, const const_iterator&); //optional
        const_iterator& operator-=(size_type); //optional            
        const_iterator operator-(size_type) const; //optional
        difference_type operator-(const_iterator) const; //optional

        reference operator*() const;
        pointer operator->() const;
        reference operator[](size_type) const; //optional
    };

    typedef std::reverse_iterator<iterator> reverse_iterator; //optional
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator; //optional

    X();
    X(const X&);
    ~X();

    X& operator=(const X&);
    bool operator==(const X&) const;
    bool operator!=(const X&) const;
    bool operator<(const X&) const; //optional
    bool operator>(const X&) const; //optional
    bool operator<=(const X&) const; //optional
    bool operator>=(const X&) const; //optional

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin(); //optional
    const_reverse_iterator rbegin() const; //optional
    const_reverse_iterator crbegin() const; //optional
    reverse_iterator rend(); //optional
    const_reverse_iterator rend() const; //optional
    const_reverse_iterator crend() const; //optional

    reference front(); //optional
    const_reference front() const; //optional
    reference back(); //optional
    const_reference back() const; //optional
    template<class ...Args>
    void emplace_front(Args&&...); //optional
    template<class ...Args>
    void emplace_back(Args&&...); //optional
    void push_front(const T&); //optional
    void push_front(T&&); //optional
    void push_back(const T&); //optional
    void push_back(T&&); //optional
    void pop_front(); //optional
    void pop_back(); //optional
    reference operator[](size_type); //optional
    const_reference operator[](size_type) const; //optional
    reference at(size_type); //optional
    const_reference at(size_type) const; //optional

    template<class ...Args>
    iterator emplace(const_iterator, Args&&...); //optional
    iterator insert(const_iterator, const T&); //optional
    iterator insert(const_iterator, T&&); //optional
    iterator insert(const_iterator, size_type, T&); //optional
    template<class iter>
    iterator insert(const_iterator, iter, iter); //optional
    iterator insert(const_iterator, std::initializer_list<T>); //optional
    iterator erase(const_iterator); //optional
    iterator erase(const_iterator, const_iterator); //optional
    void clear(); //optional
    template<class iter>
    void assign(iter, iter); //optional
    void assign(std::initializer_list<T>); //optional
    void assign(size_type, const T&); //optional

    void swap(X&);
    size_type size() const;
    size_type max_size() const;
    bool empty() const;

    A get_allocator() const; //optional
};
template <class T, class A = std::allocator<T> >
void swap(X<T,A>&, X<T,A>&); //optional


template < typename InterfaceT > 
class PipelineIterator 
{
public:
	using Interface = InterfaceT;
	using EntryType = PipelineEntry<Interface>;
private:
	EntryType* ptr;
public:
	PipelineIterator() : ptr(nullptr) {}

	PipelineIterator(const EntryType* ptr_) : ptr(const_cast<EntryType*>(ptr_)) {}

	PipelineIterator(const PipelineIterator& other) : ptr(other.ptr) {}

	PipelineIterator& operator=(const PipelineIterator& other) {
		ptr = other.ptr;
		return *this;
	}

	PipelineIterator& operator++() {
		ptr = (ptr && ptr->hasTail()) ? ptr->tail : nullptr;
		return *this;
	}

	PipelineIterator operator++(int) {
		PipelineIterator old{ this };
		const_cast<PipelineIterator*>(this)->operator++();
		return old;
	}

	PipelineIterator& operator--() {
		ptr = (ptr && ptr->hasHead()) ? ptr->head : nullptr;
		return *this;
	}

	PipelineIterator& operator--(int) {
		PipelineIterator old{ this };
		const_cast<PipelineIterator*>(this)->operator--();
		return old;
	}

	EntryType* operator->() const { return ptr; }

	operator bool() const { return static_cast<bool>(ptr); }

	friend void swap(PipelineIterator& lhs, PipelineIterator& rhs) { std::swap(lhs.ptr, rhs.ptr); }

	friend bool operator==(const PipelineIterator& lhs, const PipelineIterator& rhs) { return lhs.ptr == rhs.ptr; }

	friend bool operator!=(const PipelineIterator& lhs, const PipelineIterator& rhs) { return !(lhs == rhs); }
};


template < typename InterfaceT >
class PipelineEntry : 
	public InterfaceT,
	public MyLocal_::PipelineContainerInterface<PipelineEntry<InterfaceT>>
{
	friend class Pipeline<InterfaceT>;
	friend class PipelineIterator<InterfaceT>;

	using ContainerInterfcae = MyLocal_::PipelineContainerInterface<PipelineEntry>;

	PipelineEntry* tail;
	const PipelineEntry* head;

	inline void setTail(const PipelineEntry* tail_) { tail = const_cast<PipelineEntry*>(tail_); }

	inline void setHead(const PipelineEntry* head_) { head = const_cast<PipelineEntry*>(head_); }

protected: /// Implementation of container interface
	
	void push_back(PipelineEntry* newTail) override final {
		if (tail)
			dynamic_cast<ContainerInterfcae*>(tail)->push_back(newTail);
		else {
			tail = newTail;
			(*tail).setHead(this); 
		}
	}

	PipelineEntry* pop_back() override final {
		if (tail) {
			auto result = dynamic_cast<ContainerInterfcae*>(tail)->pop_back();
			if (result == tail) tail = nullptr;
			return result;
		} else
			return const_cast<PipelineEntry*>(this);
	}

	void push_front(PipelineEntry* newHead) override final {
		if (head)
			dynamic_cast<ContainerInterfcae*>(const_cast<PipelineEntry*>(head))->push_front(newHead);
		else {
			head = newHead;
			(*const_cast<PipelineEntry*>(head)).setTail(this); 
		}
	}

	PipelineEntry* pop_front(PipelineEntry* &newFront) override final {
		if (head) {
			auto result = dynamic_cast<ContainerInterfcae*>(const_cast<PipelineEntry*>(head))->pop_front(newFront);
			if (result == head) {
				head = nullptr;
				newFront = const_cast<PipelineEntry*>(this);
			}
			return result;
		} else 
			return const_cast<PipelineEntry*>(this);
	}

	PipelineEntry* clear() override final {
		if (tail) delete dynamic_cast<ContainerInterfcae*>(tail)->clear();
		return const_cast<PipelineEntry*>(this);
	}

public: /// Implementation of entry interface
	using Interface = InterfaceT;

	PipelineEntry() : tail(nullptr), head(nullptr) {}

	virtual ~PipelineEntry() {}

	NotCopyConstructible(PipelineEntry)

	NotCopyAssignable(PipelineEntry)

	PipelineEntry(PipelineEntry&& other) :
		head(other.head),
		tail(other.tail)
	{
		other.head = nullptr;
		other.tail = nullptr;
		if (head) (*head).setTail(this);
		if (tail) (*tail).setHead(this);
	}

	PipelineEntry& operator=(PipelineEntry&& other)
	{
		if (this == &other) return *this;
		if (head) (*head).setTail(tail);
		if (tail) (*tail).setHead(head);
		head = other.head;
		tail = other.tail;
		other.head = nullptr;
		other.tail = nullptr;
		if (head) (*head).setTail(this);
		if (tail) (*tail).setHead(this);
		return *this;
	}

	inline Interface* getTail() { return dynamic_cast<Interface*>(tail); }

	inline bool hasTail() { return static_cast<bool>(tail); }

	inline Interface* getHead() { return dynamic_cast<Interface*>(head); }

	inline bool hasHead() { return static_cast<bool>(head); }

//protected:

	// inline iterator begin() override final { 
	//     return head ? static_cast<PipelineContainerInterface*>(head)->begin() : static_cast<iterator>(this);
	// }

	// inline const_iterator cbegin() override final { 
	//     return head ? static_cast<PipelineContainerInterface*>(head)->cbegin() : static_cast<const_iterator>(this);
	// }

	// inline iterator end() override final { return nullptr; }

	// inline const_iterator cend() override final { return nullptr; }

};

//	reference back
//	pointer end
//	size_type size
//	void insert
//	

#endif