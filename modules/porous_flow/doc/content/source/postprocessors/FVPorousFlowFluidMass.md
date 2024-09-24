# FVPorousFlowFluidMass

!syntax description /Postprocessors/FVPorousFlowFluidMass

This finite volume specific `Postprocessor` calculates the mass of a fluid component $\kappa$ using
\begin{equation*}
M^{\kappa} = \phi \sum_{\beta} \chi^{\kappa}_{\beta} \rho_{\beta} S_{\beta},
\end{equation*}
where all variables are defined in [`nomenclature`](/nomenclature.md).

The fluid component $\kappa$ is specified in the input parameter [!param](/Postprocessors/FVPorousFlowFluidMass/fluid_component). By default, the mass of fluid component $\kappa$ is summed over all fluid phases. The
sum can be restricted to only a subset of fluid phases by entering the phase indexes
in the [!param](/Postprocessors/FVPorousFlowFluidMass/phase) input parameter.

This `Postprocessor` also provides the option to only calculate fluid mass below a
certain saturation, which can be invoked using the [!param](/Postprocessors/FVPorousFlowFluidMass/saturation_threshold) parameter.

The flag `use_displaced_mesh = false` is set internally by this Postprocessor, and the parameter cannot be altered by the user, even for simulations with solid-mechanical deformation.  The reason is that this Postprocessor uses the strain calculated by SolidMechanics to automatically compensate for deformed meshes.  Further information may be found [here](porous_flow/time_derivative.md).  Therefore:

- For mechanically-coupled simulations, you must provide a [!param](/Postprocessors/FVPorousFlowFluidMass/base_name) parameter that is identical to that used by the SolidMechanics strain calculator, so that this Postprocessor can retrieve the correct strain.  The most common use-case is that you provide no `base_name` to the SolidMechanics strain calculator and hence no `base_name` to this Postprocessor.
- For non-mechanically-coupled simulations, you must not provide a `base_name` that is used in any SolidMechanics strain calculators.  The most common use-case is that you have no SolidMechanics strain calculators, so you needn't worry about providing any `base_name` to this Postprocessor.  However, there is a possibility that you have a SolidMechanics strain calculator but you don't want to couple mechanics to PorousFlow.  In that case, supply `base_name = non_existent`, or similar, so that this Postprocessor doesn't retrieve any strain.

!syntax parameters /Postprocessors/FVPorousFlowFluidMass

!syntax inputs /Postprocessors/FVPorousFlowFluidMass

!syntax children /Postprocessors/FVPorousFlowFluidMass
