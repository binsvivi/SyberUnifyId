#ifndef CXSLIDEMODULE_H
#define CXSLIDEMODULE_H

#include "svm.h"
#include "cxbase.h"
#include "cxdmeans.h"

#include "cxslide.h"


class CXSlideNode : public CXDMNode
{
public:
    enum {
//        slideNode_dim = 12,
        slideNode_dim = SLIDE_FEATURE_DIM_SIZE,
//        slideNode_dim = SLIDE_GLOBAL_GRAV_MEAN_X,
        slideNode_pool_size =  300,
    };

    CXSlideNode();
    ~CXSlideNode();

    virtual void clear();
    virtual int dimSize() { return slideNode_dim; }
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
    virtual const char* typeChr() { return "slide-node"; }

    void load( svm_node *snode );
    void load( double *featVals );
    void setVal( double *pVal );
    static CXSlideNode*_newNodes( int num = 1 ) {
        CXSlideNode *tmp = NULL;
        if ( num == 1 )
            tmp = new CXSlideNode;
        else if ( num > 1 )
            tmp = new CXSlideNode[num];
        if ( tmp )
            ref += num;
        return tmp;
    }

    static void print() {
        fprintf( stderr, "CXCSNode number: %d \n", ref );
    }

protected:
    double m_data[slideNode_dim];
    static int ref;
    static int dataSize;
    static CXSlideNode* gNodePool[slideNode_pool_size];
    static int gNumNode;
};

class CxSlideModule
{
public:
    CxSlideModule();
    ~CxSlideModule();
    void clear();

    int load( const char *filePath );
    int loadFeats( const char *filePath );
    void dynaCluster(  int nCluster, int offset );

    void train();
    void trainOne();
    svm_model* trainModel( svm_problem &prob, svm_parameter &param );
    void save( const char *filePath );
    void saveModule( const char *filePath,  svm_model *pModel );
    void factored( int dimSize );
    double test( double *val, int &label );
    double test( svm_node *x, int &label );
    double testStdNode( svm_node *x, int &label );
    bool checkFeatDim( svm_node *x );

    svm_node** nodes() {
        return m_nodes;
    }
    int nodeSize() {
        return m_numFeatures;
    }
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
//    svm_model *m_model;
    svm_node **m_nodes;
    svm_node *m_features;
//    double* m_featTags;
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

void trainSlideModule();
void testSlideModule();

#endif // CXSLIDEMODULE_H
