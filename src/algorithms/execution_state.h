#pragma once

#include <map>
#include <string>
#include <vector>

struct ExecutionSnapshot {
    int step;
    std::map<std::string,int> metrics;
    std::map<std::string,std::string> details; // details optionnels pour l'operation
};

class ExecutionState {
public:
    ExecutionState() = default;

    // --- ENREGISTREMENT D'OPERATIONS ---
    void recordComparison(const std::string& a, const std::string& b);
    void recordSwap(const std::string& a, const std::string& b);
    void recordAccess(const std::string& a);

    // Enregistrement generique (utile pour AlgorithmRunner::recordStep)
    void recordOperation(const std::string& operation, const std::map<std::string,std::string>& details);

    // --- SNAPSHOT / RESTAURATION ---
    void saveState();
    void restoreState(int step);

    // --- GETTERS ---
  std::map<std::string,int> getMetrics() const;
    int getCurrentStep() const;
    int getTotalSteps() const;

    // Avancer d'une etape (utile pour l'execution pas-a-pas)
    void advanceStep();

    // --- RESET ---
    void reset();

private:
    int currentStep{0};
    int totalSteps{0};

    // Metriques individuelles (pour acces rapide) - maintenues en miroir avec `metricsMap`
    int comparisonCount{0};
    int swapCount{0};

    // Carte flexible pour toutes les metriques (source pour les snapshots)
    std::map<std::string,int> metricsMap{
        {"comparisons", 0},
        {"swaps", 0},
        {"access", 0}
    };

  // Historique complet de snapshots
    std::vector<ExecutionSnapshot> history;
};
