import React, { useState } from 'react';
import { ChevronLeft, ChevronRight } from 'lucide-react';

const QuaternionPresentation = () => {
  const [currentSlide, setCurrentSlide] = useState(0);

  const slides = [
    // Slide 1: Título
    {
      title: "Cuaterniones",
      subtitle: "Representación Matemática de Rotaciones en 3D",
      content: (
        <div className="flex flex-col items-center justify-center h-full space-y-8">
          <div className="text-6xl font-bold text-blue-600">Cuaterniones</div>
          <div className="text-2xl text-gray-600">Representación Matemática de Rotaciones en 3D</div>
          <svg className="w-64 h-64" viewBox="0 0 200 200">
            <defs>
              <linearGradient id="grad1" x1="0%" y1="0%" x2="100%" y2="100%">
                <stop offset="0%" style={{stopColor: '#3b82f6', stopOpacity: 1}} />
                <stop offset="100%" style={{stopColor: '#8b5cf6', stopOpacity: 1}} />
              </linearGradient>
            </defs>
            {/* 3D Axes */}
            <line x1="100" y1="100" x2="160" y2="140" stroke="#ef4444" strokeWidth="3" />
            <line x1="100" y1="100" x2="40" y2="140" stroke="#22c55e" strokeWidth="3" />
            <line x1="100" y1="100" x2="100" y2="40" stroke="#3b82f6" strokeWidth="3" />
            {/* Rotation arc */}
            <path d="M 100 70 A 30 30 0 0 1 120 85" fill="none" stroke="url(#grad1)" strokeWidth="4" />
            <polygon points="120,85 125,80 118,78" fill="url(#grad1)" />
            {/* Labels */}
            <text x="165" y="145" fill="#ef4444" fontSize="16" fontWeight="bold">X</text>
            <text x="30" y="145" fill="#22c55e" fontSize="16" fontWeight="bold">Y</text>
            <text x="105" y="30" fill="#3b82f6" fontSize="16" fontWeight="bold">Z</text>
          </svg>
        </div>
      )
    },
    // Slide 2: Fundamento Matemático
    {
      title: "Fundamento Matemático",
      content: (
        <div className="space-y-6">
          <div className="flex items-start space-x-4">
            <div className="flex-1">
              <p className="text-xl">Un cuaternión es una extensión de los números complejos a 4 dimensiones.</p>
              <p className="text-xl mt-2">Para rotaciones usamos <span className="font-bold text-blue-600">cuaterniones unitarios</span> (norma = 1)</p>
            </div>
            <svg className="w-32 h-32" viewBox="0 0 100 100">
              <circle cx="50" cy="50" r="40" fill="none" stroke="#3b82f6" strokeWidth="3" />
              <circle cx="50" cy="50" r="4" fill="#3b82f6" />
              <line x1="50" y1="50" x2="78" y2="32" stroke="#8b5cf6" strokeWidth="2" />
              <text x="80" y="28" fill="#8b5cf6" fontSize="10">q</text>
            </svg>
          </div>
          
          <div className="bg-blue-50 p-6 rounded-lg space-y-4">
            <div>
              <p className="font-semibold text-lg mb-2">Forma general:</p>
              <p className="text-2xl font-mono text-center">q = w + xi + yj + zk</p>
            </div>
            
            <div className="text-lg space-y-2">
              <p>Donde:</p>
              <ul className="list-disc ml-8 space-y-1">
                <li>w, x, y, z son números reales</li>
                <li>i, j, k son unidades imaginarias con: <span className="font-mono">i² = j² = k² = ijk = -1</span></li>
              </ul>
            </div>
          </div>

          <div className="bg-green-50 p-6 rounded-lg">
            <p className="font-semibold text-lg mb-2">Forma vectorial:</p>
            <p className="text-2xl font-mono text-center mb-4">q = [w, (x, y, z)] = [w, v]</p>
            <ul className="list-disc ml-8 space-y-1 text-lg">
              <li><span className="font-bold">w:</span> parte escalar (real)</li>
              <li><span className="font-bold">v = (x, y, z):</span> parte vectorial (imaginaria)</li>
            </ul>
          </div>
        </div>
      )
    },
    // Slide 3: Representación de Rotaciones
    {
      title: "Representación de Rotaciones",
      content: (
        <div className="space-y-6">
          <div className="flex items-center space-x-6">
            <div className="flex-1">
              <p className="text-xl">Una rotación de ángulo <span className="font-bold">θ</span> alrededor del eje unitario <span className="font-bold">n = (nx, ny, nz)</span></p>
            </div>
            <svg className="w-48 h-48" viewBox="0 0 150 150">
              {/* Axis */}
              <line x1="75" y1="20" x2="75" y2="130" stroke="#3b82f6" strokeWidth="3" strokeDasharray="5,5" />
              <polygon points="75,15 70,25 80,25" fill="#3b82f6" />
              <text x="80" y="15" fill="#3b82f6" fontSize="14" fontWeight="bold">n</text>
              
              {/* Rotation arc */}
              <ellipse cx="75" cy="75" rx="40" ry="15" fill="none" stroke="#8b5cf6" strokeWidth="2" />
              <path d="M 115 75 A 40 15 0 0 1 95 85" fill="none" stroke="#8b5cf6" strokeWidth="3" />
              <polygon points="95,85 100,82 97,78" fill="#8b5cf6" />
              
              {/* Angle label */}
              <text x="100" y="70" fill="#8b5cf6" fontSize="16" fontWeight="bold">θ</text>
              
              {/* Vector */}
              <line x1="75" y1="75" x2="115" y2="75" stroke="#ef4444" strokeWidth="2" />
              <circle cx="115" cy="75" r="3" fill="#ef4444" />
            </svg>
          </div>
          
          <div className="bg-purple-50 p-8 rounded-lg space-y-4">
            <p className="text-2xl font-mono text-center">q = [cos(θ/2), sin(θ/2) · n]</p>
            <p className="text-xl font-mono text-center">= [cos(θ/2), sin(θ/2)·nx, sin(θ/2)·ny, sin(θ/2)·nz]</p>
          </div>

          <div className="bg-yellow-100 border-l-4 border-yellow-500 p-6 rounded">
            <p className="font-bold text-lg mb-2">⚠️ Nota importante:</p>
            <p className="text-xl">El ángulo se divide por 2, característica fundamental de los cuaterniones.</p>
          </div>

          <div className="text-center">
            <div className="inline-block bg-gray-100 p-6 rounded-lg">
              <p className="text-lg text-gray-700">Ejemplo: Rotación de 90° → se usa 45° en la fórmula</p>
            </div>
          </div>
        </div>
      )
    },
    // Slide 4: Operaciones Fundamentales (Parte 1)
    {
      title: "Operaciones Fundamentales (1/2)",
      content: (
        <div className="space-y-6">
          <div className="flex items-center space-x-4">
            <svg className="w-32 h-32" viewBox="0 0 120 120">
              <circle cx="60" cy="60" r="35" fill="none" stroke="#3b82f6" strokeWidth="2" />
              <line x1="60" y1="60" x2="85" y2="45" stroke="#8b5cf6" strokeWidth="2" />
              <text x="88" y="43" fill="#8b5cf6" fontSize="12">q</text>
              <line x1="60" y1="60" x2="85" y2="75" stroke="#ef4444" strokeWidth="2" strokeDasharray="3,3" />
              <text x="88" y="80" fill="#ef4444" fontSize="12">q*</text>
            </svg>
            <div className="flex-1 bg-blue-50 p-6 rounded-lg">
              <p className="font-bold text-xl mb-3">Conjugado:</p>
              <p className="text-2xl font-mono text-center">q* = [w, -v] = [w, -x, -y, -z]</p>
            </div>
          </div>

          <div className="bg-green-50 p-6 rounded-lg">
            <p className="font-bold text-xl mb-3">Norma:</p>
            <p className="text-2xl font-mono text-center">||q|| = √(w² + x² + y² + z²)</p>
            <div className="mt-4 text-center">
              <svg className="inline-block w-24 h-24" viewBox="0 0 100 100">
                <circle cx="50" cy="50" r="40" fill="none" stroke="#22c55e" strokeWidth="3" />
                <line x1="50" y1="50" x2="78" y2="32" stroke="#22c55e" strokeWidth="2" />
                <text x="62" y="38" fill="#22c55e" fontSize="10">||q||=1</text>
              </svg>
            </div>
          </div>

          <div className="bg-purple-50 p-6 rounded-lg">
            <p className="font-bold text-xl mb-3">Inverso (para cuaternión unitario):</p>
            <p className="text-2xl font-mono text-center">q⁻¹ = q*</p>
            <p className="text-lg text-gray-600 text-center mt-2">(Para cuaterniones unitarios, el inverso es simplemente el conjugado)</p>
          </div>
        </div>
      )
    },
    // Slide 5: Operaciones - Multiplicación
    {
      title: "Operaciones Fundamentales (2/2)",
      content: (
        <div className="space-y-6">
          <div className="flex items-center space-x-4">
            <svg className="w-48 h-32" viewBox="0 0 180 120">
              {/* q1 */}
              <line x1="30" y1="60" x2="55" y2="40" stroke="#3b82f6" strokeWidth="2" />
              <circle cx="55" cy="40" r="3" fill="#3b82f6" />
              <text x="58" y="38" fill="#3b82f6" fontSize="12">q₁</text>
              
              {/* Arrow */}
              <text x="75" y="65" fontSize="20">×</text>
              
              {/* q2 */}
              <line x1="100" y1="60" x2="125" y2="45" stroke="#8b5cf6" strokeWidth="2" />
              <circle cx="125" cy="45" r="3" fill="#8b5cf6" />
              <text x="128" y="43" fill="#8b5cf6" fontSize="12">q₂</text>
              
              {/* Result */}
              <text x="145" y="65" fontSize="20">=</text>
              <line x1="160" y1="60" x2="175" y2="35" stroke="#ef4444" strokeWidth="2" />
              <circle cx="175" cy="35" r="3" fill="#ef4444" />
              <text x="165" y="28" fill="#ef4444" fontSize="12">q₁·q₂</text>
            </svg>
            <div className="flex-1">
              <p className="font-bold text-xl">Multiplicación:</p>
              <p className="text-lg">(composición de rotaciones)</p>
            </div>
          </div>
          
          <div className="bg-orange-50 p-6 rounded-lg">
            <p className="text-lg font-mono mb-4">q₁ · q₂ = [w₁w₂ - v₁·v₂, w₁v₂ + w₂v₁ + v₁×v₂]</p>
            
            <p className="font-semibold text-lg mt-4 mb-2">Explícitamente:</p>
            <div className="font-mono text-sm space-y-1 bg-white p-4 rounded">
              <p>q₁ · q₂ = [</p>
              <p className="ml-4">w₁w₂ - x₁x₂ - y₁y₂ - z₁z₂,</p>
              <p className="ml-4">w₁x₂ + x₁w₂ + y₁z₂ - z₁y₂,</p>
              <p className="ml-4">w₁y₂ - x₁z₂ + y₁w₂ + z₁x₂,</p>
              <p className="ml-4">w₁z₂ + x₁y₂ - y₁x₂ + z₁w₂</p>
              <p>]</p>
            </div>
          </div>

          <div className="bg-red-100 border-l-4 border-red-500 p-6 rounded flex items-center space-x-4">
            <svg className="w-16 h-16" viewBox="0 0 60 60">
              <path d="M 30 10 L 30 50" stroke="#ef4444" strokeWidth="4" />
              <path d="M 10 30 L 50 30" stroke="#ef4444" strokeWidth="4" />
            </svg>
            <div className="flex-1">
              <p className="font-bold text-lg mb-2">⚠️ IMPORTANTE:</p>
              <p className="text-xl">La multiplicación NO es conmutativa: <span className="font-mono">q₁·q₂ ≠ q₂·q₁</span></p>
            </div>
          </div>
        </div>
      )
    },
    // Slide 6: Rotación de un Vector
    {
      title: "Rotación de un Vector",
      content: (
        <div className="space-y-6">
          <div className="flex items-center space-x-4">
            <div className="flex-1">
              <p className="text-xl">Para rotar un vector <span className="font-bold">v</span> usando el cuaternión <span className="font-bold">q</span>:</p>
            </div>
            <svg className="w-40 h-40" viewBox="0 0 120 120">
              {/* Original vector */}
              <line x1="60" y1="60" x2="100" y2="60" stroke="#3b82f6" strokeWidth="3" />
              <polygon points="100,60 95,57 95,63" fill="#3b82f6" />
              <text x="102" y="58" fill="#3b82f6" fontSize="12">v</text>
              
              {/* Rotation arc */}
              <path d="M 100 60 A 40 40 0 0 1 80 92" fill="none" stroke="#8b5cf6" strokeWidth="2" strokeDasharray="3,3" />
              
              {/* Rotated vector */}
              <line x1="60" y1="60" x2="80" y2="92" stroke="#ef4444" strokeWidth="3" />
              <polygon points="80,92 82,85 76,88" fill="#ef4444" />
              <text x="75" y="102" fill="#ef4444" fontSize="12">v'</text>
              
              {/* Center */}
              <circle cx="60" cy="60" r="3" fill="#000" />
            </svg>
          </div>
          
          <div className="bg-blue-50 p-6 rounded-lg">
            <p className="font-bold text-lg mb-4">Método general:</p>
            <ol className="list-decimal ml-8 space-y-3 text-lg">
              <li>Extender v a cuaternión puro: <span className="font-mono">p = [0, v]</span></li>
              <li>Aplicar: <span className="font-mono">p' = q · p · q*</span></li>
              <li>Extraer parte vectorial de p'</li>
            </ol>
          </div>

          <div className="bg-green-50 p-6 rounded-lg">
            <p className="font-bold text-lg mb-4">Forma optimizada (evita multiplicaciones completas):</p>
            <p className="text-xl font-mono text-center mb-3">v' = v + 2w(v_q × v) + 2(v_q × (v_q × v))</p>
            <p className="text-lg text-gray-700">donde <span className="font-mono">v_q = (x, y, z)</span> es la parte vectorial de q</p>
          </div>

          <div className="bg-yellow-50 p-4 rounded-lg text-center flex items-center justify-center space-x-3">
            <svg className="w-8 h-8" viewBox="0 0 30 30">
              <circle cx="15" cy="15" r="12" fill="#fbbf24" />
              <path d="M 15 8 L 15 16 M 15 20 L 15 22" stroke="#fff" strokeWidth="2" strokeLinecap="round" />
            </svg>
            <p className="text-lg">La forma optimizada es más eficiente computacionalmente</p>
          </div>
        </div>
      )
    },
    // Slide 7: Conversión Cuaternión → Matriz
    {
      title: "Conversión: Cuaternión → Matriz",
      content: (
        <div className="space-y-6">
          <div className="flex items-center space-x-6">
            <svg className="w-32 h-32" viewBox="0 0 100 100">
              <circle cx="50" cy="50" r="35" fill="none" stroke="#3b82f6" strokeWidth="2" />
              <line x1="50" y1="50" x2="75" y2="38" stroke="#8b5cf6" strokeWidth="2" />
              <text x="78" y="36" fill="#8b5cf6" fontSize="12">q</text>
            </svg>
            <svg className="w-12 h-12" viewBox="0 0 40 40">
              <polygon points="10,20 30,20 25,15 30,20 25,25" fill="#666" />
            </svg>
            <svg className="w-32 h-32" viewBox="0 0 100 100">
              <rect x="20" y="20" width="20" height="20" fill="#ef4444" opacity="0.3" />
              <rect x="40" y="20" width="20" height="20" fill="#22c55e" opacity="0.3" />
              <rect x="60" y="20" width="20" height="20" fill="#3b82f6" opacity="0.3" />
              <rect x="20" y="40" width="20" height="20" fill="#ef4444" opacity="0.3" />
              <rect x="40" y="40" width="20" height="20" fill="#22c55e" opacity="0.3" />
              <rect x="60" y="40" width="20" height="20" fill="#3b82f6" opacity="0.3" />
              <rect x="20" y="60" width="20" height="20" fill="#ef4444" opacity="0.3" />
              <rect x="40" y="60" width="20" height="20" fill="#22c55e" opacity="0.3" />
              <rect x="60" y="60" width="20" height="20" fill="#3b82f6" opacity="0.3" />
              <text x="45" y="95" fill="#000" fontSize="12">R</text>
            </svg>
          </div>
          
          <p className="text-xl mb-4">Cuaternión → Matriz de Rotación:</p>
          
          <div className="bg-blue-50 p-6 rounded-lg overflow-x-auto">
            <div className="font-mono text-lg">
              <p className="text-center mb-2">R = </p>
              <div className="grid grid-cols-3 gap-2 text-sm">
                <div className="bg-white p-2 rounded text-center">1-2(y²+z²)</div>
                <div className="bg-white p-2 rounded text-center">2(xy-wz)</div>
                <div className="bg-white p-2 rounded text-center">2(xz+wy)</div>
                <div className="bg-white p-2 rounded text-center">2(xy+wz)</div>
                <div className="bg-white p-2 rounded text-center">1-2(x²+z²)</div>
                <div className="bg-white p-2 rounded text-center">2(yz-wx)</div>
                <div className="bg-white p-2 rounded text-center">2(xz-wy)</div>
                <div className="bg-white p-2 rounded text-center">2(yz+wx)</div>
                <div className="bg-white p-2 rounded text-center">1-2(x²+y²)</div>
              </div>
            </div>
          </div>

          <div className="bg-gray-100 p-6 rounded-lg">
            <p className="font-bold text-lg mb-2">Nota:</p>
            <p className="text-lg">Esta conversión es útil cuando se necesita enviar transformaciones a APIs gráficas que trabajan con matrices.</p>
          </div>
        </div>
      )
    },
    // Slide 8: Conversión Matriz → Cuaternión
    {
      title: "Conversión: Matriz → Cuaternión",
      content: (
        <div className="space-y-6">
          <div className="flex items-center space-x-6">
            <svg className="w-32 h-32" viewBox="0 0 100 100">
              <rect x="20" y="20" width="20" height="20" fill="#ef4444" opacity="0.3" />
              <rect x="40" y="20" width="20" height="20" fill="#22c55e" opacity="0.3" />
              <rect x="60" y="20" width="20" height="20" fill="#3b82f6" opacity="0.3" />
              <rect x="20" y="40" width="20" height="20" fill="#ef4444" opacity="0.3" />
              <rect x="40" y="40" width="20" height="20" fill="#22c55e" opacity="0.3" />
              <rect x="60" y="40" width="20" height="20" fill="#3b82f6" opacity="0.3" />
              <rect x="20" y="60" width="20" height="20" fill="#ef4444" opacity="0.3" />
              <rect x="40" y="60" width="20" height="20" fill="#22c55e" opacity="0.3" />
              <rect x="60" y="60" width="20" height="20" fill="#3b82f6" opacity="0.3" />
              <text x="45" y="95" fill="#000" fontSize="12">R</text>
            </svg>
            <svg className="w-12 h-12" viewBox="0 0 40 40">
              <polygon points="10,20 30,20 25,15 30,20 25,25" fill="#666" />
            </svg>
            <svg className="w-32 h-32" viewBox="0 0 100 100">
              <circle cx="50" cy="50" r="35" fill="none" stroke="#3b82f6" strokeWidth="2" />
              <line x1="50" y1="50" x2="75" y2="38" stroke="#8b5cf6" strokeWidth="2" />
              <text x="78" y="36" fill="#8b5cf6" fontSize="12">q</text>
            </svg>
          </div>
          
          <p className="text-xl mb-4">Matriz → Cuaternión:</p>
          
          <div className="bg-green-50 p-6 rounded-lg">
            <div className="font-mono text-xl space-y-3">
              <p>w = √(1 + r₁₁ + r₂₂ + r₃₃) / 2</p>
              <p>x = (r₃₂ - r₂₃) / (4w)</p>
              <p>y = (r₁₃ - r₃₁) / (4w)</p>
              <p>z = (r₂₁ - r₁₂) / (4w)</p>
            </div>
          </div>

          <div className="bg-yellow-100 border-l-4 border-yellow-500 p-6 rounded">
            <p className="font-bold text-lg mb-2">⚠️ Advertencia:</p>
            <p className="text-lg">Esta es una de las fórmulas posibles. Existen métodos más robustos numéricamente que manejan casos especiales cuando w es cercano a cero.</p>
          </div>

          <div className="bg-blue-50 p-4 rounded-lg text-center">
            <p className="text-lg">💡 Donde rᵢⱼ son los elementos de la matriz de rotación R</p>
          </div>
        </div>
      )
    },
    // Slide 9: SLERP
    {
      title: "Interpolación: SLERP",
      content: (
        <div className="space-y-6">
          <div className="flex items-center space-x-4">
            <div className="flex-1">
              <p className="text-xl"><span className="font-bold">SLERP</span> (Spherical Linear Interpolation) permite interpolar suavemente entre dos rotaciones:</p>
            </div>
            <svg className="w-48 h-48" viewBox="0 0 150 150">
              <circle cx="75" cy="75" r="60" fill="none" stroke="#e5e7eb" strokeWidth="2" />
              
              {/* q1 */}
              <line x1="75" y1="75" x2="115" y2="45" stroke="#3b82f6" strokeWidth="3" />
              <circle cx="115" cy="45" r="4" fill="#3b82f6" />
              <text x="118" y="43" fill="#3b82f6" fontSize="12">q₁</text>
              
              {/* q2 */}
              <line x1="75" y1="75" x2="105" y2="105" stroke="#8b5cf6" strokeWidth="3" />
              <circle cx="105" cy="105" r="4" fill="#8b5cf6" />
              <text x="108" y="110" fill="#8b5cf6" fontSize="12">q₂</text>
              
              {/* Arc between them */}
              <path d="M 115 45 A 60 60 0 0 1 105 105" fill="none" stroke="#ef4444" strokeWidth="2" strokeDasharray="5,3" />
              
              {/* Intermediate point */}
              <line x1="75" y1="75" x2="128" y2="75" stroke="#22c55e" strokeWidth="2" />
              <circle cx="128" cy="75" r="3" fill="#22c55e" />
              <text x="130" y="72" fill="#22c55e" fontSize="10">t=0.5</text>
            </svg>
          </div>
          
          <div className="bg-purple-50 p-6 rounded-lg">
            <p className="text-xl font-mono text-center mb-4">
              slerp(q₁, q₂, t) = sin((1-t)Ω)/sin(Ω) · q₁ + sin(tΩ)/sin(Ω) · q₂
            </p>
          </div>

          <div className="bg-blue-50 p-6 rounded-lg space-y-3">
            <p className="font-bold text-lg">Donde:</p>
            <ul className="list-disc ml-8 space-y-2 text-lg">
              <li><span className="font-mono">Ω = arccos(q₁ · q₂)</span> es el ángulo entre cuaterniones</li>
              <li><span className="font-mono">t ∈ [0, 1]</span> es el parámetro de interpolación</li>
            </ul>
          </div>

          <div className="bg-green-100 p-6 rounded-lg flex items-center space-x-4">
            <svg className="w-16 h-16" viewBox="0 0 60 60">
              <circle cx="30" cy="30" r="25" fill="#22c55e" opacity="0.2" />
              <path d="M 20 30 L 27 37 L 42 22" fill="none" stroke="#22c55e" strokeWidth="3" strokeLinecap="round" strokeLinejoin="round" />
            </svg>
            <div>
              <p className="font-bold text-lg mb-2">✨ Ventaja Principal:</p>
              <p className="text-xl">SLERP produce rotaciones uniformes y suaves, ideal para animaciones.</p>
            </div>
          </div>
        </div>
      )
    },
    // Slide 10: Ventajas
    {
      title: "Ventajas de los Cuaterniones",
      content: (
        <div className="grid grid-cols-2 gap-4">
          <div className="bg-green-50 p-4 rounded-lg border-l-4 border-green-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <rect x="10" y="15" width="8" height="25" fill="#22c55e" />
              <rect x="21" y="20" width="8" height="20" fill="#22c55e" />
              <rect x="32" y="10" width="8" height="30" fill="#22c55e" />
            </svg>
            <div>
              <p className="font-bold text-lg">Compacto</p>
              <p className="text-gray-700">Solo 4 números (vs 9 de matrices)</p>
            </div>
          </div>
          
          <div className="bg-green-50 p-4 rounded-lg border-l-4 border-green-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <circle cx="25" cy="25" r="15" fill="none" stroke="#22c55e" strokeWidth="3" />
              <path d="M 25 10 L 25 25 L 35 20" fill="none" stroke="#22c55e" strokeWidth="2" />
            </svg>
            <div>
              <p className="font-bold text-lg">Sin gimbal lock</p>
              <p className="text-gray-700">No tiene singularidades</p>
            </div>
          </div>
          
          <div className="bg-green-50 p-4 rounded-lg border-l-4 border-green-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <path d="M 5 40 Q 15 10, 25 25 T 45 25" fill="none" stroke="#22c55e" strokeWidth="3" />
            </svg>
            <div>
              <p className="font-bold text-lg">Interpolación suave</p>
              <p className="text-gray-700">SLERP produce rotaciones uniformes</p>
            </div>
          </div>
          
          <div className="bg-green-50 p-4 rounded-lg border-l-4 border-green-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <polygon points="25,5 45,40 5,40" fill="none" stroke="#22c55e" strokeWidth="3" />
              <text x="20" y="30" fill="#22c55e" fontSize="18">⚡</text>
            </svg>
            <div>
              <p className="font-bold text-lg">Eficiente</p>
              <p className="text-gray-700">Composición y normalización rápidas</p>
            </div>
          </div>
          
          <div className="bg-green-50 p-4 rounded-lg border-l-4 border-green-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <rect x="10" y="10" width="30" height="30" fill="none" stroke="#22c55e" strokeWidth="3" />
              <path d="M 15 25 L 22 32 L 35 18" fill="none" stroke="#22c55e" strokeWidth="2" />
            </svg>
            <div>
              <p className="font-bold text-lg">Estable numéricamente</p>
              <p className="text-gray-700">Fácil mantener unitario</p>
            </div>
          </div>
          
          <div className="bg-green-50 p-4 rounded-lg border-l-4 border-green-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <circle cx="15" cy="25" r="8" fill="#22c55e" opacity="0.5" />
              <circle cx="25" cy="25" r="8" fill="#22c55e" opacity="0.5" />
              <circle cx="35" cy="25" r="8" fill="#22c55e" opacity="0.5" />
            </svg>
            <div>
              <p className="font-bold text-lg">Menos operaciones</p>
              <p className="text-gray-700">Para composición que matrices</p>
            </div>
          </div>
        </div>
      )
    },
    // Slide 11: Desventajas
    {
      title: "Desventajas de los Cuaterniones",
      content: (
        <div className="space-y-4">
          <div className="bg-red-50 p-4 rounded-lg border-l-4 border-red-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <circle cx="25" cy="25" r="18" fill="none" stroke="#ef4444" strokeWidth="3" />
              <text x="18" y="33" fill="#ef4444" fontSize="24">?</text>
            </svg>
            <div>
              <p className="font-bold text-lg">Menos intuitivo</p>
              <p className="text-gray-700">Difícil de visualizar mentalmente</p>
            </div>
          </div>
          
          <div className="bg-red-50 p-4 rounded-lg border-l-4 border-red-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <line x1="25" y1="10" x2="25" y2="25" stroke="#ef4444" strokeWidth="2" />
              <circle cx="25" cy="25" r="4" fill="#ef4444" />
              <line x1="25" y1="25" x2="25" y2="40" stroke="#ef4444" strokeWidth="2" strokeDasharray="3,2" />
              <circle cx="25" cy="40" r="4" fill="#ef4444" opacity="0.5" />
              <text x="8" y="15" fill="#ef4444" fontSize="10">q</text>
              <text x="8" y="44" fill="#ef4444" fontSize="10">-q</text>
            </svg>
            <div>
              <p className="font-bold text-lg">Doble cobertura</p>
              <p className="text-gray-700">q y -q representan la misma rotación</p>
            </div>
          </div>
          
          <div className="bg-red-50 p-4 rounded-lg border-l-4 border-red-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <text x="5" y="25" fill="#ef4444" fontSize="16">q₁</text>
              <text x="18" y="25" fill="#ef4444" fontSize="16">×</text>
              <text x="28" y="25" fill="#ef4444" fontSize="16">q₂</text>
              <text x="18" y="40" fill="#ef4444" fontSize="16">≠</text>
              <text x="5" y="15" fill="#ef4444" fontSize="16">q₂</text>
              <text x="18" y="15" fill="#ef4444" fontSize="16">×</text>
              <text x="28" y="15" fill="#ef4444" fontSize="16">q₁</text>
            </svg>
            <div>
              <p className="font-bold text-lg">No conmutativa</p>
              <p className="text-gray-700">El orden importa en las composiciones</p>
            </div>
          </div>
          
          <div className="bg-red-50 p-4 rounded-lg border-l-4 border-red-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <circle cx="25" cy="25" r="18" fill="none" stroke="#ef4444" strokeWidth="2" />
              <path d="M 25 10 A 15 15 0 0 1 40 25" fill="none" stroke="#ef4444" strokeWidth="2" />
              <polygon points="40,25 37,20 35,26" fill="#ef4444" />
            </svg>
            <div>
              <p className="font-bold text-lg">Normalización periódica</p>
              <p className="text-gray-700">Para mantener la norma unitaria</p>
            </div>
          </div>
          
          <div className="bg-red-50 p-4 rounded-lg border-l-4 border-red-500 flex items-start space-x-3">
            <svg className="w-12 h-12 flex-shrink-0" viewBox="0 0 50 50">
              <rect x="5" y="10" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="13" y="10" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="21" y="10" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="5" y="18" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="13" y="18" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="21" y="18" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="5" y="26" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="13" y="26" width="8" height="8" fill="#ef4444" opacity="0.3" />
              <rect x="21" y="26" width="8" height="8" fill="#ef4444" opacity="0.3" />
            </svg>
            <div>
              <p className="font-bold text-lg">Conversión a matriz</p>
              <p className="text-gray-700">Necesaria para algunos cálculos gráficos</p>
            </div>
          </div>
        </div>
      )
    },
    // Slide 12: Aplicaciones
    {
      title: "Aplicaciones de Cuaterniones",
      content: (
        <div className="grid grid-cols-2 gap-4">
          <div className="bg-blue-50 p-5 rounded-lg flex items-start space-x-4">
            <svg className="w-16 h-16 flex-shrink-0" viewBox="0 0 60 60">
              <rect x="10" y="25" width="40" height="25" fill="#3b82f6" opacity="0.3" />
              <circle cx="30" cy="18" r="8" fill="#3b82f6" />
              <rect x="5" y="50" width="10" height="5" fill="#3b82f6" />
              <rect x="45" y="50" width="10" height="5" fill="#3b82f6" />
            </svg>
            <div>
              <p className="font-bold text-xl mb-2">Animación 3D</p>
              <p className="text-gray-700 text-lg">Interpolación suave para movimientos realistas</p>
            </div>
          </div>
          
          <div className="bg-purple-50 p-5 rounded-lg flex items-start space-x-4">
            <svg className="w-16 h-16 flex-shrink-0" viewBox="0 0 60 60">
              <rect x="15" y="15" width="30" height="30" rx="3" fill="#8b5cf6" opacity="0.3" />
              <circle cx="25" cy="25" r="3" fill="#8b5cf6" />
              <circle cx="35" cy="25" r="3" fill="#8b5cf6" />
              <rect x="22" y="35" width="16" height="3" fill="#8b5cf6" />
            </svg>
            <div>
              <p className="font-bold text-xl mb-2">Videojuegos</p>
              <p className="text-gray-700 text-lg">Control de cámaras y personajes</p>
            </div>
          </div>
          
          <div className="bg-green-50 p-5 rounded-lg flex items-start space-x-4">
            <svg className="w-16 h-16 flex-shrink-0" viewBox="0 0 60 60">
              <circle cx="30" cy="30" r="20" fill="none" stroke="#22c55e" strokeWidth="3" />
              <circle cx="30" cy="30" r="3" fill="#22c55e" />
              <line x1="30" y1="30" x2="45" y2="20" stroke="#22c55e" strokeWidth="2" />
            </svg>
            <div>
              <p className="font-bold text-xl mb-2">Simulación física</p>
              <p className="text-gray-700 text-lg">Orientación de objetos</p>
            </div>
          </div>
          
          <div className="bg-yellow-50 p-5 rounded-lg flex items-start space-x-4">
            <svg className="w-16 h-16 flex-shrink-0" viewBox="0 0 60 60">
              <rect x="20" y="15" width="20" height="15" fill="#eab308" opacity="0.3" />
              <rect x="15" y="30" width="30" height="3" fill="#eab308" />
              <line x1="10" y1="33" x2="5" y2="45" stroke="#eab308" strokeWidth="2" />
              <line x1="50" y1="33" x2="55" y2="45" stroke="#eab308" strokeWidth="2" />
            </svg>
            <div>
              <p className="font-bold text-xl mb-2">Control de actitud</p>
              <p className="text-gray-700 text-lg">Satélites y drones</p>
            </div>
          </div>
          
          <div className="bg-pink-50 p-5 rounded-lg flex items-start space-x-4">
            <svg className="w-16 h-16 flex-shrink-0" viewBox="0 0 60 60">
              <rect x="10" y="20" width="18" height="12" rx="2" fill="#ec4899" opacity="0.3" />
              <rect x="32" y="20" width="18" height="12" rx="2" fill="#ec4899" opacity="0.3" />
              <line x1="28" y1="26" x2="32" y2="26" stroke="#ec4899" strokeWidth="2" />
            </svg>
            <div>
              <p className="font-bold text-xl mb-2">Realidad virtual</p>
              <p className="text-gray-700 text-lg">Seguimiento de movimientos</p>
            </div>
          </div>
          
          <div className="bg-orange-50 p-5 rounded-lg flex items-start space-x-4">
            <svg className="w-16 h-16 flex-shrink-0" viewBox="0 0 60 60">
              <circle cx="30" cy="30" r="18" fill="none" stroke="#f97316" strokeWidth="2" strokeDasharray="5,3" />
              <circle cx="30" cy="30" r="3" fill="#f97316" />
              <line x1="30" y1="30" x2="30" y2="12" stroke="#f97316" strokeWidth="2" />
              <polygon points="30,12 27,17 33,17" fill="#f97316" />
            </svg>
            <div>
              <p className="font-bold text-xl mb-2">Sistemas de seguimiento</p>
              <p className="text-gray-700 text-lg">Muchas composiciones de rotaciones</p>
            </div>
          </div>
        </div>
      )
    },
    // Slide 13: Tabla Comparativa (Parte 1)
    {
      title: "Comparación: Dimensión y Memoria",
      content: (
        <div className="space-y-4">
          <div className="flex justify-center space-x-6 mb-6">
            <svg className="w-24 h-24" viewBox="0 0 80 80">
              <rect x="10" y="10" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="25" y="10" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="40" y="10" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="10" y="25" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="25" y="25" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="40" y="25" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="10" y="40" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="25" y="40" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <rect x="40" y="40" width="15" height="15" fill="#3b82f6" opacity="0.3" />
              <text x="20" y="70" fill="#3b82f6" fontSize="10">Matrices</text>
            </svg>
            <svg className="w-24 h-24" viewBox="0 0 80 80">
              <circle cx="40" cy="20" r="8" fill="#22c55e" opacity="0.3" />
              <circle cx="25" cy="40" r="8" fill="#22c55e" opacity="0.3" />
              <circle cx="55" cy="40" r="8" fill="#22c55e" opacity="0.3" />
              <text x="25" y="70" fill="#22c55e" fontSize="10">Euler</text>
            </svg>
            <svg className="w-24 h-24" viewBox="0 0 80 80">
              <circle cx="30" cy="30" r="6" fill="#8b5cf6" opacity="0.3" />
              <circle cx="50" cy="30" r="6" fill="#8b5cf6" opacity="0.3" />
              <circle cx="30" cy="45" r="6" fill="#8b5cf6" opacity="0.3" />
              <circle cx="50" cy="45" r="6" fill="#8b5cf6" opacity="0.3" />
              <text x="15" y="70" fill="#8b5cf6" fontSize="10">Cuaterniones</text>
            </svg>
          </div>
          
          <table className="w-full border-collapse">
            <thead>
              <tr className="bg-gray-200">
                <th className="border border-gray-400 p-3 text-left">Característica</th>
                <th className="border border-gray-400 p-3 text-center">Matrices</th>
                <th className="border border-gray-400 p-3 text-center">Euler</th>
                <th className="border border-gray-400 p-3 text-center">Cuaterniones</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Dimensión</td>
                <td className="border border-gray-400 p-3 text-center">9 valores (3×3)</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">3 valores</td>
                <td className="border border-gray-400 p-3 text-center">4 valores</td>
              </tr>
              <tr className="bg-gray-50">
                <td className="border border-gray-400 p-3 font-semibold">Grados de libertad</td>
                <td className="border border-gray-400 p-3 text-center">3 (con 6 restricciones)</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">3</td>
                <td className="border border-gray-400 p-3 text-center">3 (con 1 restricción)</td>
              </tr>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Memoria</td>
                <td className="border border-gray-400 p-3 text-center">36 bytes</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">12 bytes</td>
                <td className="border border-gray-400 p-3 text-center">16 bytes</td>
              </tr>
              <tr className="bg-gray-50">
                <td className="border border-gray-400 p-3 font-semibold">Intuitividad</td>
                <td className="border border-gray-400 p-3 text-center">Media</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Alta</td>
                <td className="border border-gray-400 p-3 text-center">Baja</td>
              </tr>
            </tbody>
          </table>
        </div>
      )
    },
    // Slide 14: Tabla Comparativa (Parte 2) - Gimbal Lock
    {
      title: "Comparación: Problemas y Singularidades",
      content: (
        <div className="space-y-4">
          <div className="flex justify-center mb-6">
            <svg className="w-64 h-48" viewBox="0 0 200 150">
              {/* Gimbal lock illustration */}
              <ellipse cx="100" cy="75" rx="80" ry="30" fill="none" stroke="#ef4444" strokeWidth="2" opacity="0.3" />
              <ellipse cx="100" cy="75" rx="60" ry="60" fill="none" stroke="#3b82f6" strokeWidth="2" />
              <ellipse cx="100" cy="75" rx="80" ry="30" fill="none" stroke="#22c55e" strokeWidth="2" transform="rotate(90 100 75)" />
              <line x1="20" y1="75" x2="180" y2="75" stroke="#ef4444" strokeWidth="2" strokeDasharray="5,3" />
              <text x="70" y="130" fill="#ef4444" fontSize="12">Gimbal Lock</text>
            </svg>
          </div>
          
          <table className="w-full border-collapse">
            <thead>
              <tr className="bg-gray-200">
                <th className="border border-gray-400 p-3 text-left">Característica</th>
                <th className="border border-gray-400 p-3 text-center">Matrices</th>
                <th className="border border-gray-400 p-3 text-center">Euler</th>
                <th className="border border-gray-400 p-3 text-center">Cuaterniones</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Gimbal Lock</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">❌ No sufre</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">✅ Sí, problema crítico</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">❌ No sufre</td>
              </tr>
              <tr className="bg-gray-50">
                <td className="border border-gray-400 p-3 font-semibold">Singularidades</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Ninguna</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">En ±90° del eje medio</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Ninguna</td>
              </tr>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Ambigüedad</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Única</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">Múltiples</td>
                <td className="border border-gray-400 p-3 text-center">Doble (q y -q)</td>
              </tr>
              <tr className="bg-gray-50">
                <td className="border border-gray-400 p-3 font-semibold">Continuidad</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Continua</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">Discontinuidades</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Continua</td>
              </tr>
            </tbody>
          </table>
        </div>
      )
    },
    // Slide 15: Tabla Comparativa (Parte 3)
    {
      title: "Comparación: Operaciones",
      content: (
        <div className="space-y-4">
          <table className="w-full border-collapse text-sm">
            <thead>
              <tr className="bg-gray-200">
                <th className="border border-gray-400 p-3 text-left">Característica</th>
                <th className="border border-gray-400 p-3 text-center">Matrices</th>
                <th className="border border-gray-400 p-3 text-center">Euler</th>
                <th className="border border-gray-400 p-3 text-center">Cuaterniones</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Composición</td>
                <td className="border border-gray-400 p-3 text-center">Mult. matriz (27m, 18s)</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">Conversión primero</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Mult. (16m, 12s)</td>
              </tr>
              <tr className="bg-gray-50">
                <td className="border border-gray-400 p-3 font-semibold">Aplicar a vector</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Directa (9m, 6s)</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">Conversión necesaria</td>
                <td className="border border-gray-400 p-3 text-center">Fórmula (15m, 15s)</td>
              </tr>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Interpolación</td>
                <td className="border border-gray-400 p-3 text-center">Difícil, no lineal</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">Muy problemática</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">SLERP: excelente</td>
              </tr>
              <tr className="bg-gray-50">
                <td className="border border-gray-400 p-3 font-semibold">Inversión</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Transposición</td>
                <td className="border border-gray-400 p-3 text-center">Invertir orden/signos</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Conjugado</td>
              </tr>
            </tbody>
          </table>
          <p className="text-sm text-gray-600 italic">m = multiplicaciones, s = sumas</p>
        </div>
      )
    },
    // Slide 16: Tabla Comparativa (Parte 4)
    {
      title: "Comparación: Estabilidad y Rendimiento",
      content: (
        <div className="space-y-4">
          <div className="flex justify-center space-x-8 mb-6">
            <div className="text-center">
              <svg className="w-24 h-24 mx-auto" viewBox="0 0 80 80">
                <path d="M 10 70 L 25 40 L 40 55 L 55 25 L 70 45" fill="none" stroke="#3b82f6" strokeWidth="3" />
                <circle cx="10" cy="70" r="3" fill="#3b82f6" />
                <circle cx="25" cy="40" r="3" fill="#3b82f6" />
                <circle cx="40" cy="55" r="3" fill="#3b82f6" />
                <circle cx="55" cy="25" r="3" fill="#3b82f6" />
                <circle cx="70" cy="45" r="3" fill="#3b82f6" />
              </svg>
              <p className="text-sm mt-2">Rendimiento</p>
            </div>
            <div className="text-center">
              <svg className="w-24 h-24 mx-auto" viewBox="0 0 80 80">
                <rect x="15" y="50" width="15" height="20" fill="#22c55e" opacity="0.7" />
                <rect x="32.5" y="35" width="15" height="35" fill="#22c55e" opacity="0.7" />
                <rect x="50" y="20" width="15" height="50" fill="#22c55e" opacity="0.7" />
              </svg>
              <p className="text-sm mt-2">Estabilidad</p>
            </div>
          </div>
          
          <table className="w-full border-collapse">
            <thead>
              <tr className="bg-gray-200">
                <th className="border border-gray-400 p-3 text-left">Característica</th>
                <th className="border border-gray-400 p-3 text-center">Matrices</th>
                <th className="border border-gray-400 p-3 text-center">Euler</th>
                <th className="border border-gray-400 p-3 text-center">Cuaterniones</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Estabilidad numérica</td>
                <td className="border border-gray-400 p-3 text-center">Puede perder ortogonalidad</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Estable</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Muy estable</td>
              </tr>
              <tr className="bg-gray-50">
                <td className="border border-gray-400 p-3 font-semibold">Velocidad composición</td>
                <td className="border border-gray-400 p-3 text-center">Media</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">Lenta</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Rápida</td>
              </tr>
              <tr>
                <td className="border border-gray-400 p-3 font-semibold">Uso GPU</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Excelente (hardware nativo)</td>
                <td className="border border-gray-400 p-3 text-center bg-red-100">Pobre</td>
                <td className="border border-gray-400 p-3 text-center bg-green-100">Bueno</td>
              </tr>
            </tbody>
          </table>
        </div>
      )
    },
    // Slide 17: Resumen
    {
      title: "Resumen",
      content: (
        <div className="space-y-6">
          <div className="bg-blue-50 p-6 rounded-lg">
            <div className="flex items-start space-x-4">
              <svg className="w-16 h-16 flex-shrink-0" viewBox="0 0 60 60">
                <circle cx="30" cy="30" r="25" fill="#3b82f6" opacity="0.2" />
                <path d="M 20 30 L 27 37 L 42 22" fill="none" stroke="#3b82f6" strokeWidth="3" strokeLinecap="round" strokeLinejoin="round" />
              </svg>
              <div className="flex-1">
                <p className="font-bold text-2xl mb-4">🎯 Puntos Clave</p>
                <ul className="space-y-3 text-lg">
                  <li className="flex items-start">
                    <span className="mr-3">•</span>
                    <span>Los cuaterniones son una representación eficiente y estable de rotaciones 3D</span>
                  </li>
                  <li className="flex items-start">
                    <span className="mr-3">•</span>
                    <span>Evitan el gimbal lock y no tienen singularidades</span>
                  </li>
                  <li className="flex items-start">
                    <span className="mr-3">•</span>
                    <span>SLERP permite interpolación suave entre rotaciones</span>
                  </li>
                  <li className="flex items-start">
                    <span className="mr-3">•</span>
                    <span>Son ideales para animación, videojuegos y simulaciones</span>
                  </li>
                </ul>
              </div>
            </div>
          </div>

          <div className="grid grid-cols-2 gap-4">
            <div className="bg-green-50 p-6 rounded-lg">
              <div className="flex items-center space-x-3 mb-4">
                <svg className="w-12 h-12" viewBox="0 0 50 50">
                  <circle cx="25" cy="25" r="20" fill="#22c55e" opacity="0.2" />
                  <text x="15" y="33" fill="#22c55e" fontSize="24">✓</text>
                </svg>
                <p className="font-bold text-2xl">Cuándo Usar</p>
              </div>
              <ul className="space-y-2 text-lg">
                <li>• Interpolar rotaciones</li>
                <li>• Evitar gimbal lock</li>
                <li>• Muchas composiciones</li>
                <li>• Estabilidad numérica</li>
              </ul>
            </div>

            <div className="bg-yellow-50 p-6 rounded-lg">
              <div className="flex items-center space-x-3 mb-4">
                <svg className="w-12 h-12" viewBox="0 0 50 50">
                  <circle cx="25" cy="25" r="20" fill="#eab308" opacity="0.2" />
                  <text x="18" y="33" fill="#eab308" fontSize="24">⚠</text>
                </svg>
                <p className="font-bold text-2xl">Alternativas</p>
              </div>
              <ul className="space-y-2 text-lg">
                <li>• Intuitividad (Euler)</li>
                <li>• Hardware GPU (Matrices)</li>
                <li>• Pocas composiciones</li>
              </ul>
            </div>
          </div>
        </div>
      )
    },
    // Slide 18: Fin
    {
      title: "Fin",
      content: (
        <div className="flex flex-col items-center justify-center h-full space-y-8">
          <svg className="w-48 h-48" viewBox="0 0 200 200">
            <defs>
              <linearGradient id="grad2" x1="0%" y1="0%" x2="100%" y2="100%">
                <stop offset="0%" style={{stopColor: '#3b82f6', stopOpacity: 1}} />
                <stop offset="50%" style={{stopColor: '#8b5cf6', stopOpacity: 1}} />
                <stop offset="100%" style={{stopColor: '#ec4899', stopOpacity: 1}} />
              </linearGradient>
            </defs>
            <circle cx="100" cy="100" r="80" fill="none" stroke="url(#grad2)" strokeWidth="4" />
            <circle cx="100" cy="100" r="60" fill="none" stroke="url(#grad2)" strokeWidth="3" opacity="0.6" />
            <circle cx="100" cy="100" r="40" fill="none" stroke="url(#grad2)" strokeWidth="2" opacity="0.4" />
            <circle cx="100" cy="100" r="8" fill="url(#grad2)" />
            
            {/* Rotating vectors */}
            <line x1="100" y1="100" x2="160" y2="60" stroke="#3b82f6" strokeWidth="3" />
            <line x1="100" y1="100" x2="140" y2="140" stroke="#8b5cf6" strokeWidth="3" opacity="0.7" />
            <line x1="100" y1="100" x2="60" y2="140" stroke="#ec4899" strokeWidth="3" opacity="0.5" />
          </svg>
          
          <div className="text-6xl">🎓</div>
          <div className="text-5xl font-bold bg-gradient-to-r from-blue-600 via-purple-600 to-pink-600 bg-clip-text text-transparent">
            ¡Gracias!
          </div>
          <div className="text-2xl text-gray-600">Cuaterniones: Matemáticas para Rotaciones 3D</div>
          <div className="mt-8 text-xl text-gray-500 flex items-center space-x-3">
            <svg className="w-8 h-8" viewBox="0 0 40 40">
              <circle cx="20" cy="20" r="15" fill="none" stroke="#6b7280" strokeWidth="2" />
              <text x="15" y="25" fill="#6b7280" fontSize="16">?</text>
            </svg>
            <span>¿Preguntas?</span>
          </div>
        </div>
      )
    }
  ];

  const nextSlide = () => {
    if (currentSlide < slides.length - 1) {
      setCurrentSlide(currentSlide + 1);
    }
  };

  const prevSlide = () => {
    if (currentSlide > 0) {
      setCurrentSlide(currentSlide - 1);
    }
  };

  const goToSlide = (index) => {
    setCurrentSlide(index);
  };

  return (
    <div className="w-full h-screen bg-gradient-to-br from-gray-50 to-gray-100 flex flex-col">
      {/* Main Slide Area */}
      <div className="flex-1 flex items-center justify-center p-8">
        <div className="w-full max-w-6xl h-full bg-white rounded-2xl shadow-2xl p-12 flex flex-col">
          {/* Slide Title */}
          {slides[currentSlide].title && currentSlide !== 0 && currentSlide !== slides.length - 1 && (
            <div className="mb-6 pb-4 border-b-2 border-blue-500">
              <h1 className="text-4xl font-bold text-gray-800">{slides[currentSlide].title}</h1>
              {slides[currentSlide].subtitle && (
                <p className="text-xl text-gray-600 mt-2">{slides[currentSlide].subtitle}</p>
              )}
            </div>
          )}
          
          {/* Slide Content */}
          <div className="flex-1 overflow-y-auto">
            {slides[currentSlide].content}
          </div>
        </div>
      </div>

      {/* Navigation Controls */}
      <div className="bg-white border-t border-gray-200 px-8 py-4">
        <div className="max-w-6xl mx-auto flex items-center justify-between">
          {/* Previous Button */}
          <button
            onClick={prevSlide}
            disabled={currentSlide === 0}
            className={`flex items-center space-x-2 px-6 py-3 rounded-lg font-semibold transition-all ${
              currentSlide === 0
                ? 'bg-gray-200 text-gray-400 cursor-not-allowed'
                : 'bg-blue-500 text-white hover:bg-blue-600 shadow-md hover:shadow-lg'
            }`}
          >
            <ChevronLeft size={24} />
            <span>Anterior</span>
          </button>

          {/* Slide Indicators */}
          <div className="flex items-center space-x-2">
            {slides.map((_, index) => (
              <button
                key={index}
                onClick={() => goToSlide(index)}
                className={`w-3 h-3 rounded-full transition-all ${
                  index === currentSlide
                    ? 'bg-blue-500 w-8'
                    : 'bg-gray-300 hover:bg-gray-400'
                }`}
                title={`Ir a diapositiva ${index + 1}`}
              />
            ))}
          </div>

          {/* Next Button */}
          <button
            onClick={nextSlide}
            disabled={currentSlide === slides.length - 1}
            className={`flex items-center space-x-2 px-6 py-3 rounded-lg font-semibold transition-all ${
              currentSlide === slides.length - 1
                ? 'bg-gray-200 text-gray-400 cursor-not-allowed'
                : 'bg-blue-500 text-white hover:bg-blue-600 shadow-md hover:shadow-lg'
            }`}
          >
            <span>Siguiente</span>
            <ChevronRight size={24} />
          </button>
        </div>

        {/* Slide Counter */}
        <div className="text-center mt-3 text-gray-600 font-medium">
          Diapositiva {currentSlide + 1} de {slides.length}
        </div>
      </div>
    </div>
  );
};

export default QuaternionPresentation;