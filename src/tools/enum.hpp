#ifndef ENUMS_HPP
#define ENUMS_HPP

/// @brief Defines calculation modes for the program
enum class TargetMode {
    None,            ///< No calculation selected
    IntegrateCube,   ///< Electron density integration
    AcceptorNPDonor  ///< Full donor–acceptor–nanoparticle system
};

#endif // ENUMS_HPP

