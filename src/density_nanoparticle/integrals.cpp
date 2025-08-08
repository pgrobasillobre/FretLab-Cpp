#include "integrals.hpp"
#include "target.hpp"
#include "density.hpp"
#include "parameters.hpp"

///
/// @brief Computes the Coulomb and overlap integrals between acceptor and donor densities.
///
void Integrals::acceptor_donor(const Target &target, const Density &acceptor, const Density &donor)
{
  int n_acc = acceptor.n_points_reduced;
  int n_don = donor.n_points_reduced;

  // Direct references (no deep copies). Needed for multithreaded performance.
  const std::vector<double> &rho_acc = acceptor.rho_reduced;
  const std::vector<double> &rho_don = donor.rho_reduced;
  const std::vector<std::array<double, 3>> &xyz_acc = acceptor.xyz;
  const std::vector<std::array<double, 3>> &xyz_don = donor.xyz;

  double int_coulomb = 0.0;
  double int_overlap = 0.0;

  for (int i = 0; i < n_acc; ++i)
  {
    for (int j = 0; j < n_don; ++j)
    {
      if (target.calc_overlap_int && i == j)
      {
        int_overlap += rho_acc[i] * rho_don[j];
      }

      double dx = xyz_acc[i][0] - xyz_don[j][0];
      double dy = xyz_acc[i][1] - xyz_don[j][1];
      double dz = xyz_acc[i][2] - xyz_don[j][2];

      double dist2 = dx * dx + dy * dy + dz * dz;
      double dist = std::sqrt(dist2);

      if (dist <= 1.0e-14)
        continue;

      // Screening factor to avoid singularities
      // QMscrnFact is a constant defined in Parameters
      double invdist = 1.0 / dist;
      double sf = dist / Parameters::QMscrnFact;
      double screen_pot = std::erf(sf);

      int_coulomb += rho_acc[i] * rho_don[j] * invdist * screen_pot;
    }
  }

  coulomb_acceptor_donor = int_coulomb;
  if (target.calc_overlap_int) overlap_acceptor_donor = -target.omega_0 * int_overlap;

}
