#ifndef CXGESTUREMODULE_H
#define CXGESTUREMODULE_H

#include "svm.h"
#include "cxbase.h"
#include "cxdmeans.h"

class CXGestureNode : public CXDMNode
{
public:
    enum {
        sdm_dim = 14,
    };

    CXGestureNode();
    ~CXGestureNode();

    virtual void clear();
    virtual int dimSize() { return sdm_dim; }
    virtual double* val();
    virtual double distanceTo( CXDMNode *target );
    virtual CXDMNode& operator = ( CXDMNode &target );
    virtual CXDMNode& operator += ( CXDMNode &target );
    virtual CXDMNode& operator += ( CXDMNode *target );
    virtual CXDMNode& operator /= ( double div );
    virtual void zero();
    virtual CXDMNode* copy();
    virtual CXDMNode* newNodes( int num = 1 );
    virtual CXDMNode** newNodePointers( int num = 1 );
    virtual const char* typeChr() { return "gesture-node"; }

    void load( svm_node *snode );
    void load( double *featVals );
    void setVal( double *pVal );
    static CXGestureNode*_newNodes( int num = 1 ) {
        CXGestureNode *tmp = NULL;
        if ( num == 1 )
            tmp = new CXGestureNode;
        else if ( num > 1 )
            tmp = new CXGestureNode[num];
        if ( tmp )
            ref += num;
        return tmp;
    }

protected:
    double m_data[sdm_dim];
    static int ref;
    static int dataSize;
};

class CxGestureModule
{
public:
    CxGestureModule();
    ~CxGestureModule();
    void clear();

    int loadModule( const char *filePath );
    int loadFeats( const char *filePath );

 //   void train();
    void saveModule( const char *filePath );
    void factored( int dimSize );
    void dynaCluster( int nCluster, int offset );
    void train();
    svm_model* trainModel( svm_problem &prob, svm_parameter &param );
    void trainOne();
    void save( const char *filePath );
  //  double test( svm_node *x );
    double test( svm_node *x , int &label);
    double testStdNode( svm_node *x, int &label );
    int load( const char *filePath );
    svm_node** nodes() {
        return m_nodes;
    }
    int nodeSize() {
        return m_numFeatures;
    }
//    svm_model* module() {
 //       return m_model;
//    }

    svm_model* module( int i = 0 ) {
        if ( i >= 0 && i < m_nModule)
            return m_modules[i];
        else
            return NULL;
    }

protected:
    void releaseFeatures();
    void releaseModule();
    void readFeature( FILE *fp, svm_node *node, int size );

protected:
    svm_model *m_model;
    svm_node **m_nodes;
    svm_node *m_features;
    double* m_featTags;
    int m_numFeatures;

    svm_model **m_modules;
    int m_nModule;
    cxskmeans m_kmeans;

    double *m_probs;
    double *m_meanVals;
    double *m_factors;
    bool m_bFactor;

    int m_featDimSize;

};

void trainGestureModule();
void testGestureModule();

#endif // CXGESTUREMODULE_H
