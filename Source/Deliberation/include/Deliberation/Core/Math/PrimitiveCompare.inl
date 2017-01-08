namespace deliberation
{

bool PrimitivesEqual(const Sphere & lhs, const Sphere & rhs, float tolerance)
{
    auto positionDelta = glm::abs(lhs.position() - rhs.position());

    return positionDelta.x <= tolerance && positionDelta.y <= tolerance && positionDelta.y <= tolerance &&
        std::abs(lhs.radius() - rhs.radius()) <= tolerance;
}

}