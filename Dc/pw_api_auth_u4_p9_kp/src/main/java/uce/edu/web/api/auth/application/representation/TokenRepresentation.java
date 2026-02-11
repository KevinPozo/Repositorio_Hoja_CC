package uce.edu.web.api.auth.application.representation;

public class TokenRepresentation {
    public String accessToken;
    public long expiresAt;
    public String role;

    public TokenRepresentation() {
    }

    public TokenRepresentation(String accessToken, long expiresAt, String role) {
        this.accessToken = accessToken;
        this.expiresAt = expiresAt;
        this.role = role;
    }
}
