# -*- coding: utf-8 -*-
"""
Created on Fri Sep 19 09:08:18 2025

@author: user
"""

import streamlit as st
import numpy as np

st.title("Demo sencilla con Streamlit")

entrada = st.text_input("Introduce números separados por comas:")

if entrada:
    try:
        numeros = [float(x.strip()) for x in entrada.split(",")]
        media = np.mean(numeros)
        st.write(f"Has introducido: {numeros}")
        st.success(f"La media aritmética es: {media:.2f}")
    except ValueError:
        st.error("Por favor, introduce solo números separados por comas.")
