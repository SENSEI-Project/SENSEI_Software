# -*- coding: utf-8 -*-
"""
Created on Fri Sep 19 09:44:01 2025

@author: user
"""

import streamlit as st
import wntr

st.title("Demo con WNTR - Red fija")

try:
    # Cargar la red fija Red1.inp (debe estar en la misma carpeta que este script)
    wn = wntr.network.WaterNetworkModel("Red1.inp")

    # Contar nodos y enlaces
    num_nodos = len(wn.junction_name_list) + len(wn.reservoir_name_list) + len(wn.tank_name_list)
    num_links = len(wn.pipe_name_list) + len(wn.pump_name_list) + len(wn.valve_name_list)

    st.success(f"La red 'Red1.inp' tiene {num_nodos} nodos y {num_links} enlaces.")

    # Mostrar detalle
    st.write("**Nodos:**")
    st.write(f"- Junctions: {len(wn.junction_name_list)}")
    st.write(f"- Tanks: {len(wn.tank_name_list)}")
    st.write(f"- Reservoirs: {len(wn.reservoir_name_list)}")

    st.write("**Enlaces:**")
    st.write(f"- Pipes: {len(wn.pipe_name_list)}")
    st.write(f"- Pumps: {len(wn.pump_name_list)}")
    st.write(f"- Valves: {len(wn.valve_name_list)}")

except Exception as e:
    st.error(f"No se pudo cargar Red1.inp. Error: {e}")
