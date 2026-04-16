#include "DominanceChecker.h"

void LocalCheckRulebook::add_node(RealizationPairPtr rp) {
    auto id = rp->id;
    const RulebookGraph& rulebook_graph = rp->rulebook_graph;
    if (!transfer_equal_to_less_than(rp)) {
        for (auto it = G_equal[id].begin(); it != G_equal[id].end(); ) {
            if (rulebook_graph.is_dominated_equal((*it)->rule_apex->f, rp->rule_apex->f)) {
                it = G_equal[id].erase(it);
            } else {
                it ++;
            }
        }
    }

    G_equal[id].push_front(rp);
}


bool LocalCheckRulebook::is_dominated(RealizationPairPtr rp, bool transferFlag = false) {
    auto id = rp->id;
    const RulebookGraph& rulebook_graph = rp->rulebook_graph;

    if (!transferFlag || !transfer_equal_to_less_than(rp)) {
        for (auto ap : G_equal[id]) {
            if (rulebook_graph.is_dominated_equal(rp->rule_apex->f, ap->rule_apex->f)) {
                return true;
            }
        }
    }

    for (auto ap : G_less_than[id]) {
        if (rulebook_graph.is_dominated_less_than(rp->rule_apex->f, ap->rule_apex->f)) {
            return true;
        }
    }

    return false;
}

bool LocalCheckRulebook::transfer_equal_to_less_than(RealizationPairPtr rp) {
    auto id = rp->id;
    const RulebookGraph& rulebook_graph = rp->rulebook_graph;
    auto ordered_rules = rp->rulebook_graph.get_ordered_rules();
    auto topmost_rule = ordered_rules[0];

    if (G_equal[id].size() > 0 && (*G_equal[id].begin())->rule_apex->f[topmost_rule] < rp->rule_apex->f[topmost_rule]) {
        for (auto tr : G_equal[id]) {
            for (auto it = G_less_than[id].begin(); it != G_less_than[id].end(); ){
                if (rulebook_graph.is_dominated_equal((*it)->rule_apex->f, tr->rule_apex->f)) {
                    it = G_less_than[id].erase(it);
                } else {
                    it ++;
                }
            }
            G_less_than[id].push_front(tr);
        }
        G_equal[id].clear();
        return true;
    }
    assert(G_equal[id].size() == 0 || (*G_equal[id].begin())->rule_apex->f[topmost_rule] == rp->rule_apex->f[topmost_rule]);
    return false;
}


bool SolutionCheckRulebook::is_dominated(RealizationPairPtr rp, bool transferFlag = false) {
    auto id = rp->id;
    const RulebookGraph& rulebook_graph = rp->rulebook_graph;

    if (!transferFlag || !transfer_equal_to_less_than(rp)) {
        for (auto ap : solutions_equal) {
            if (ap->update_rule_apex_by_merge_if_bounded(rp->rule_apex, eps)) {
                return true;
            }
        }
    }

    for (auto ap : solutions_less_than) {
        if (ap->update_rule_apex_by_merge_if_bounded(rp->rule_apex, eps)) {
            return true;
        }
    }

    return false;
}

bool SolutionCheckRulebook::transfer_equal_to_less_than(RealizationPairPtr rp) {
    auto id = rp->id;
    const RulebookGraph& rulebook_graph = rp->rulebook_graph;
    auto ordered_rules = rp->rulebook_graph.get_ordered_rules();
    auto topmost_rule = ordered_rules[0];

    if (solutions_equal.size() > 0 && (*solutions_equal.begin())->rule_apex->f[topmost_rule] < rp->rule_apex->f[topmost_rule]) {
        for (auto tr : solutions_equal) {
            for (auto it = solutions_less_than.begin(); it != solutions_less_than.end(); ){
                if (rulebook_graph.is_dominated_equal((*it)->rule_apex->f, tr->rule_apex->f)) {
                    it = solutions_less_than.erase(it);
                } else {
                    it ++;
                }
            }
            solutions_less_than.push_front(tr);
        }
        solutions_equal.clear();
        return true;
    }
    assert(solutions_equal.size() == 0 || (*solutions_equal.begin())->rule_apex->f[topmost_rule] == rp->rule_apex->f[topmost_rule]);
    return false;
}

void SolutionCheckRulebook::add_node(RealizationPairPtr node) {
    auto id = node->id;
    const RulebookGraph& rulebook_graph = node->rulebook_graph;
    if (!transfer_equal_to_less_than(node)) {
        for (auto it = solutions_equal.begin(); it != solutions_equal.end(); ) {
            if (rulebook_graph.is_dominated_equal((*it)->rule_apex->f, node->rule_apex->f)) {
                it = solutions_equal.erase(it);
            } else {
                it ++;
            }
        }
    }

    solutions_equal.push_front(node);
}


bool LocalCheckRulebookBasic::is_dominated(RealizationPairPtr rp, bool transferFlag = false) {
    auto id = rp->id;
    const RulebookGraph& rulebook_graph = rp->rulebook_graph;
    for (auto ap : G[id]) {
        if (rulebook_graph.dominates(ap->rule_apex->f, rp->rule_apex->f)) {
            return true;
        }
    }
    return false;
}

void LocalCheckRulebookBasic::add_node(RealizationPairPtr node) {
    auto id = node->id;
    const RulebookGraph& rulebook_graph = node->rulebook_graph;
    for (auto it = G[id].begin(); it != G[id].end(); ) {
        if (rulebook_graph.dominates(node->rule_apex->f, (*it)->rule_apex->f)) {
            it = G[id].erase(it);
        } else {
            it ++;
        }
    }
    G[id].push_front(node);
}


bool SolutionCheckRulebookBasic::is_dominated(RealizationPairPtr rp, bool transferFlag = false) {
    auto id = rp->id;
    const RulebookGraph& rulebook_graph = rp->rulebook_graph;

    for (auto ap : solutions) {
        if (ap->update_rule_apex_by_merge_if_bounded(rp->rule_apex, eps)) {
            return true;
        }
    }
    return false;
}
void SolutionCheckRulebookBasic::add_node(RealizationPairPtr node) {
    auto id = node->id;
    const RulebookGraph& rulebook_graph = node->rulebook_graph;
    for (auto it = solutions.begin(); it != solutions.end(); ) {
        if (rulebook_graph.dominates(node->rule_apex->f, (*it)->rule_apex->f)) {
            it = solutions.erase(it);
        } else {
            it ++;
        }
    }
    solutions.push_front(node);
}