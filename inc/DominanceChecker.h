#pragma once

#include "Utils/Definitions.h"

// class DominanceChecker {
// protected:
//     EPS eps;
// public:
//     virtual ~DominanceChecker(){};
//     DominanceChecker(EPS eps):eps(eps){};

//     virtual bool is_dominated(ApexPathPairPtr node) = 0;

//     virtual void add_node(ApexPathPairPtr ap) = 0;
// };

class RulebookDominanceChecker {
protected:
    EPS eps;
public:
    virtual ~RulebookDominanceChecker(){};
    RulebookDominanceChecker(EPS eps):eps(eps){};

    virtual bool is_dominated(RealizationPairPtr rp, bool transferFlag) = 0;

    virtual void add_node(RealizationPairPtr rp) = 0;
};

class LocalCheckRulebook: public RulebookDominanceChecker{

protected:
    std::vector<std::list<RealizationPairPtr>> G_equal;
    std::vector<std::list<RealizationPairPtr>> G_less_than;

public:

    LocalCheckRulebook(EPS eps, size_t graph_size): RulebookDominanceChecker(eps), G_equal(graph_size + 1), G_less_than(graph_size + 1) {};

    bool transfer_equal_to_less_than(RealizationPairPtr rp);
    virtual bool is_dominated(RealizationPairPtr rp, bool transferFlag);
    virtual void add_node(RealizationPairPtr rp);
};

// No Dimensionality Reduction
class LocalCheckRulebookBasic: public RulebookDominanceChecker{

protected:
    std::vector<std::list<RealizationPairPtr>> G;

public:

    LocalCheckRulebookBasic(EPS eps, size_t graph_size): RulebookDominanceChecker(eps), G(graph_size+1){};

    virtual bool is_dominated(RealizationPairPtr rp, bool transferFlag);
    virtual void add_node(RealizationPairPtr rp);
};

class SolutionCheckRulebook: public RulebookDominanceChecker{
protected:
    std::list<RealizationPairPtr> solutions_equal;
    std::list<RealizationPairPtr> solutions_less_than;

public:

    SolutionCheckRulebook(EPS eps): RulebookDominanceChecker(eps) {};

    bool transfer_equal_to_less_than(RealizationPairPtr rp);
    virtual bool is_dominated(RealizationPairPtr rp, bool transferFlag);
    virtual void add_node(RealizationPairPtr rp);
};

// No Dimensionality Reduction
class SolutionCheckRulebookBasic: public RulebookDominanceChecker{
protected:
    std::list<RealizationPairPtr> solutions;

public:

    SolutionCheckRulebookBasic(EPS eps): RulebookDominanceChecker(eps) {};

    virtual bool is_dominated(RealizationPairPtr rp, bool transferFlag);
    virtual void add_node(RealizationPairPtr rp);
};