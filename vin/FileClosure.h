

extern void my_exit(int eno);

class FpClosure {
	FILE *fp;
	char fname[128];

	void appendLeaf(const char* leaf) {
		if (fname[strlen(fname)-1] != '/'){
			strcat(fname, "/");
		}
		strcat(fname, leaf);
	}
	void init(const char* root) {
		strcpy(fname, root);
	}	
	void open(const char* mode) {
		fp = fopen(fname, mode);
		if (!fp){
			err("FAILED to open \"%s\" for \"%s\"",
			    fname, mode);
			my_exit(-errno);
		}

//		fprintf(stderr, "FpClosure \"%s\" fp %p\n", fname, fp);
	}
 public:
	FpClosure(const char* path, const char* basename, const char* mode) {
		init(path);
		appendLeaf(basename);
		open(mode);
	}
	FpClosure(const char* path, 
		  const char* n1, const char* n2, const char* mode) {
		init(path);
		appendLeaf(n1);
		appendLeaf(n2);
		open(mode);
	}
	FILE* getFp() { return fp; }
	~FpClosure() {
		fclose(fp);
	}
};


class WriteClosure : public FpClosure {
 public:
	WriteClosure(const char* root, const char* file) :
		FpClosure(root, file, "write", "w") {}
};

class ReadClosure : public FpClosure {
 public:
	ReadClosure(const char* root, const char* file) :
		FpClosure(root, file, "read", "r") {}
};

class FileAccessor {
	char root[80];
	char name[80];

 public:
	FileAccessor(const char* _root, const char* _name) {
		strcpy(root, _root);
		strcpy(name, _name);
	}

	int fileExists(void) {
		if (ReadClosure(root, name).getFp() == 0){
			err("path \"%s/%s\" does not exist", root, name);
			return 0;
		}else{
			return 1;
		}
	}
        void setValue(unsigned value) {
		WriteClosure fc(root, name);
		fprintf(fc.getFp(), "%04x", value);
	}
	void getValue(unsigned *value) {
		ReadClosure fc(root, name);
		fscanf(fc.getFp(), "%x", value);
	}
	void setValue(const char* value) {
		WriteClosure fc(root, name);
		fprintf(fc.getFp(), "%s", value);
	}
	void getValue(char *value, int maxlen) {
		ReadClosure fc(root, name);
		fgets(value, maxlen, fc.getFp());
	}
};

