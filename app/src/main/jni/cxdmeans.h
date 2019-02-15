#ifndef CXKMEANS_H
#define CXKMEANS_H

#include "cxbase.h"

class CXDMNode {
public:
    CXDMNode() {}
    virtual ~CXDMNode() {}

public:
    virtual void clear() {}
    virtual int dimSize() { return 0; }
    virtual double* val() { return NULL; }
    virtual double distanceTo( CXDMNode *target ) {
#ifdef CXDF_QT
        Q_UNUSED(target)
#endif
        return 0.;
    }
    virtual CXDMNode& operator = ( CXDMNode &target ) {
#ifdef CXDF_QT
        Q_UNUSED(target)
#endif
        return *this; }
    virtual CXDMNode& operator += ( CXDMNode &target ) {
#ifdef CXDF_QT
        Q_UNUSED(target)
#endif
        return *this; }
    virtual CXDMNode& operator += ( CXDMNode *target ) {
#ifdef CXDF_QT
        Q_UNUSED(target)
#endif
        return *this; }
    virtual CXDMNode& operator /= ( double div ) {
#ifdef CXDF_QT
        Q_UNUSED(div)
#endif
        return *this; }
    virtual void zero() {}
    virtual CXDMNode* copy() { return NULL; }
    virtual CXDMNode* newNodes( int num = 1 ) {
#ifdef CXDF_QT
        Q_UNUSED(num)
#endif
        return NULL; }
    virtual CXDMNode** newNodePointers( int num = 1 ) {
#ifdef CXDF_QT
        Q_UNUSED(num)
#endif
        return NULL; }
    virtual const char* typeChr() { return "dm-node"; }
    bool isType( CXDMNode *target ) {
        if ( !strcmp( this->typeChr(), target->typeChr() ) )
             return true;
        else
            return false;
 }
};

void freeDynaNode( CXDMNode *nodes, int num );

typedef struct cxskmeans{
    cxskmeans() {
        means = NULL;
        counts = NULL;
        nCluster = 0;

        labels = NULL;
        nLables = 0;

        meanVals = NULL;
        nDimSize = 0;

        dist = 0.;
    }
    ~cxskmeans() {
        clear();
    }

    void clear() {
        if ( means ) {
            for ( int i = 0; i < nCluster; i++ ) {
                means[i]->clear();
                delete means[i];
            }
//            free( means );
//            means = NULL;
            CXDFFreePointers( means );
        }
        CXDFFreeInt( counts );
        nCluster = 0;

        CXDFFreeInt( labels );
        nLables = 0;

        CXDFFreeDouble( meanVals);
        nDimSize = 0;

        dist = 0.;
    }

    void initWithData( CXDMNode **node, int k ) {
        if ( !node || !(*node) )
            return;
        if ( nCluster == k && means && *means && (*means)->isType(*node ) )
            return;
        clear();
        means = (*node)->newNodePointers( k );
        nCluster = k;
    }

    void setLabels(  int n ) {
        if ( nLables != n ) {
            CXDFFreeInt( labels );
            nLables = n;
            if ( nLables > 0 )
                labels = CXDFMallocInt( nLables );
        }
    }

    void zeroMeans() {
        if ( means ) {
            for ( int i = 0; i < nCluster; i++ ) {
                means[i]->zero();
            }
        }
    }
    void reset() {
        means = NULL;
        counts = NULL;
        nCluster = 0;
        labels = NULL;
        nLables = 0;
        meanVals = NULL;
        nDimSize = 0;
        dist = 0.;   
    }
    void initCounts() {
        CXDFFreeInt( counts );
        counts = CXDFMallocInt( nCluster );
    }

    void copy( cxskmeans *target ) {
        clear();
        initWithData( target->means, target->nCluster );
        setLabels( target->nLables );
        for ( int i = 0; i < nCluster; i++ )
            *means[i] = *target->means[i];
        if ( labels )
            memcpy( labels, target->labels, sizeof(int) * nLables );
        if ( target->counts ) {
            initCounts();
            memcpy( counts, target->counts,sizeof(int) * nCluster  );
        }

        dist = target->dist;
    }

    cxskmeans& operator = ( cxskmeans &node ) {
        means = node.means;
        counts = node.counts;
        nCluster = node.nCluster;
        labels = node.labels;
        nLables = nLables;
        return *this;
    }

    cxskmeans& operator = ( cxskmeans *node ) {
        means = node->means;
        counts = node->counts;
        nCluster = node->nCluster;
        labels = node->labels;
        nLables = nLables;
        return *this;
    }

    void initKMeans( int k, int nDim ) {
        nCluster = k;
        nDimSize = nDim;
        meanVals = CXDFMallocDouble( nCluster * nDimSize );
    }

    CXDMNode **means; //k 个 means vector
    int *counts;
    int nCluster; //k 个 class

    int *labels; //N 个sample's label
    int nLables;
    //for read kmeans file
    double *meanVals;
    int nDimSize;

    double dist; //sigma( dist(sample to it' class mean) ), 聚类评价
}cxskmeans;

void readKMeans( cxskmeans &kmeans, FILE *fp );
void printKMeans( cxskmeans &kmeans, FILE *fp );
void loadKMeans( cxskmeans &kmeans, const char *filePath );
void saveKMeans( cxskmeans &kmeans, const char *filePath );

class CXDmeans
{
public:
    CXDmeans();
    ~CXDmeans();
    void clear();
    //input: 样本data, num; 聚类个数 k , 范围 offset; + 默认参数
    void dynamicCluster( CXDMNode **data, int num, int k, int offset, cxskmeans *dst = NULL );
    //input: 样本data, num; 聚类个数 k , 范围 offset; + 参数: 最大迭代次数 maxIterNum, 终止阈值 endError
    void dynamicCluster( CXDMNode **data, int num, int k, int offset, int maxIterNum, double endError, cxskmeans *dst = NULL );
    //初始化聚类中心
    //input: 样本data, num; 聚类个数 k
    void init( CXDMNode **data, int num, cxskmeans *km );
    //聚类,默认参数
    void cluster( cxskmeans *km );
    //聚类 + 默认输入数据 + 参数 + 数据缓冲区
    void cluster( cxskmeans *km, int iterNum, double endError,
                  int *counts = NULL, CXDMNode **next_means = NULL );
     //聚类 + 输入数据 + 参数 + 数据缓冲区
    void cluster( cxskmeans *km, CXDMNode **data, int num,
                  int *counts = NULL, CXDMNode **next_means = NULL );
    void cluster( cxskmeans *km, CXDMNode **data, int num, int maxIterNum, double endError,
                  int *counts = NULL, CXDMNode **next_means = NULL );
    //聚类迭代子过程
    double update( cxskmeans *km, CXDMNode **data, int num, int *counts, CXDMNode **next_means );
    //优化聚类种子中心初始化, 找到局部最优聚类种子分布
    void optimumInit( CXDMNode **data, int num, cxskmeans *km, int *counts = NULL, CXDMNode **next_means = NULL );
    //返回样本最近聚类标签 + 距离
    static double getLabel( CXDMNode *x, cxskmeans *km, int k, int &label );
    //取随即量
    double getRand( int N );
    //计算所有样本的均值中心点
    void computeMean( CXDMNode **data, int num );
    //计算整体损失函数
    void computeGlobalCost( cxskmeans *km );

    cxskmeans* kmeans() {
        return m_KMean;
    }

    inline void setKMeans( cxskmeans *km ) {
        m_KMean = km;
    }
    inline void resetKMeans() {
        m_KMean = NULL;
    }

protected:
    cxskmeans *m_KMean;
    //input datas
    CXDMNode **m_datas;
    int m_num;
    //global Mean
    CXDMNode *m_gMean;

    int m_maxIterNum;
    double m_endError;
};

#endif // CXKMEANS_H
