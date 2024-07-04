#include "match.h"

void Match::printMatch() const {
        cout << "Circuit 1 PO Name: " << circuit_1_po_name << endl;
        cout << "Circuit 2 PO Name: " << circuit_2_po_name << endl;
        cout << "Circuit 1 Negation: " << (circuit_1_negation ? "True" : "False") << endl;
        cout << "Circuit 2 Negation: " << (circuit_2_negation ? "True" : "False") << endl;

        cout << "PI Match: " << endl;
        for (const auto& pi : pi_match) {
            cout << "PI 1 - Name: " << pi.first.first << ", Negation: " << (pi.first.second ? "True" : "False") << " and ";
            cout << "PI 2 - Name: " << pi.second.first << ", Negation: " << (pi.second.second ? "True" : "False") << endl;
        }
}

void Match::setMatch(vector<pair<pair<string, bool>, pair<string, bool>>>& pi1) {
        pi_match = pi1;
    }