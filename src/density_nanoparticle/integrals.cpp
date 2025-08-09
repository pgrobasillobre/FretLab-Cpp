#include "integrals.hpp"
#include "target.hpp"
#include "density.hpp"
#include "parameters.hpp"

#include <cmath>
#include <omp.h>

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

  bool calc_overlap  = target.calc_overlap_int;

  // QMscrnFact is a constant defined in Parameters
  const double inv_QMscrnFact = 1.0 / Parameters::QMscrnFact;

  // For parallel computation if OMP is ON
  #pragma omp parallel for reduction(+:int_coulomb,int_overlap) schedule(static)
  for (int i = 0; i < n_acc; ++i)
  {
    for (int j = 0; j < n_don; ++j)
    {
      if (calc_overlap && i == j)
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
      double invdist = 1.0 / dist;
      double sf = dist * inv_QMscrnFact;
      double screen_pot = std::erf(sf);

      int_coulomb += rho_acc[i] * rho_don[j] * invdist * screen_pot;
    }
  }

  coulomb_acceptor_donor = int_coulomb;
  if (calc_overlap) overlap_acceptor_donor = -target.omega_0 * int_overlap;

}
