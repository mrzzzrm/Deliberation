    for (auto & uniform : m_program.layout().uniforms())
    {
        d_unsetUniformLocations.insert(uniform.location());
    }
