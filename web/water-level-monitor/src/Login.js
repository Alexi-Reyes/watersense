import React, { useState } from 'react';

const Login = ({ onLoginSuccess }) => {
  const [email, setEmail] = useState('');
  const [error, setError] = useState('');

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError('');

    if (!email) {
      setError('Please enter your email.');
      return;
    }

    onLoginSuccess(email);
  };

  return (
    <div className="login-container">
      <h2>Enter Your Email to View Water Level Data</h2>
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label htmlFor="email">Email:</label>
          <input
            type="email"
            id="email"
            value={email}
            onChange={(e) => setEmail(e.target.value)}
            required
          />
        </div>
        <button type="submit">View Data</button>
        {error && <p className="error-message">{error}</p>}
      </form>
    </div>
  );
};

export default Login;
